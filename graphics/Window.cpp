#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <random>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Window.h"


const int DEFAULT_BUFFER_SIZE = 1200;

void error_callback(int error, const char* description);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void Window::initVertexData()
{
	// Texture buffer
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Type buffer
	glGenVertexArrays(1, &typeVAO);
	glGenBuffers(1, &typeVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, typeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glBindVertexArray(typeVAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Window::initShaders()
{
	manager.generateShader("sprite", "./graphics/shaders/sprite.vert", "./graphics/shaders/sprite.frag");
	Shader spriteShader = manager.getShader("sprite");

	glm::mat4 projection = glm::ortho(0.0f, (float)this->width, (float)this->height, 0.0f, -1.0f, 1.0f);
	spriteShader.use();
	spriteShader.setInt("image", 0);
	spriteShader.setMatrix4("projection", projection);

	manager.generateShader("type", "./graphics/shaders/type.vert", "./graphics/shaders/type.frag");
	Shader typeShader = manager.getShader("type");
	typeShader.use();
	typeShader.setMatrix4("projection", projection);
}

void Window::initFreeType(std::string fontName, int renderSize)
{
	std::string fontPathBegin = "./graphics/fonts/";
	std::string fontExtension = ".ttf";
	std::string fontPath = fontPathBegin + fontName + fontExtension;
	if (FT_Init_FreeType(&FTLib))
	{
		std::cout << "Error initializing FreeType!" << std::endl;
		return;
	}

	if (FT_New_Face(FTLib, fontPath.c_str(), 0, &currentFace))
	{
		std::cout << "Error initializing face!" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(currentFace, 0, renderSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(currentFace, c, FT_LOAD_RENDER))
		{
			std::cout << "Failed to render char " << c << std::endl;
			continue;
		}
		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, currentFace->glyph->bitmap.width, currentFace->glyph->bitmap.rows, 
			0, GL_RED, GL_UNSIGNED_BYTE, currentFace->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		FaceCharacter character(tex, currentFace->glyph->advance.x, 
			glm::ivec2(currentFace->glyph->bitmap.width, currentFace->glyph->bitmap.rows), 
			glm::ivec2(currentFace->glyph->bitmap_left, currentFace->glyph->bitmap_top));
		faceCharacters.emplace(c, character);
	}

	FT_Done_Face(currentFace);
	FT_Done_FreeType(FTLib);

}

void Window::draw(std::string spriteName, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color)
{
	Shader shader = manager.getShader("sprite");
	shader.use();
	Sprite toRender = manager.getSprite(spriteName);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.setMatrix4("model", model);
	shader.setVector3f("spriteColor", color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0);
	toRender.bind();

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Window::drawUI(float playableWidth, float playableHeight, float offsetWidth, float offsetHeight, bool gameIsPaused, int playerHealth, int enemyHealth, int score)
{
	draw("solid-square", glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f, glm::vec3(0.5, 0.5, 0.5));
	draw("solid-square", glm::vec2(offsetWidth, offsetHeight), glm::vec2(playableWidth, playableHeight), 0.0f, glm::vec3(0, 0, 0));

	if (gameIsPaused)
	{
		renderText("PAUSED", offsetWidth + (playableWidth / 2), offsetHeight + (playableHeight / 2), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	std::string playerHPString = "Lives: ";
	
	float playerTextWidth = renderText(playerHPString, offsetWidth,
		(offsetHeight + playableHeight) + ((width - (offsetHeight + playableHeight)) / 2),
		0.5f, glm::vec3(0.5, 0.3, 0.2)) + offsetWidth;
	for (int lives = 0; lives < playerHealth; lives++)
	{
		draw("solid-square", glm::vec2(playerTextWidth, (offsetHeight + playableHeight) + ((width - (offsetHeight + playableHeight)) / 2))
			, glm::vec2(50, 50), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

float Window::renderText(std::string toRender, float x, float y, float scale, glm::vec3 color)
{
	float totalWidth = 0.0f;

	Shader typeShader = manager.getShader("type");
	typeShader.use();
	typeShader.setVector3f("textColor", color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->typeVAO);

	for (unsigned char c : toRender)
	{
		FaceCharacter fc = faceCharacters.at(c);

		float pos_x = x + fc.bearing.x * scale;
		float pos_y = y - fc.bearing.y * scale;
		float width = fc.size.x * scale;
		float height = fc.size.y * scale;

		float vertices[6][4] = 
		{
			{pos_x,			pos_y,			0.0f, 0.0f},
			{pos_x,			pos_y + height,	0.0f, 1.0f},
			{pos_x + width, pos_y + height,	1.0f, 1.0f},

			{pos_x,			pos_y,			0.0f, 0.0f},
			{pos_x + width, pos_y + height,	1.0f, 1.0f},
			{pos_x + width, pos_y,			1.0f, 0.0f},
		};

		glBindTexture(GL_TEXTURE_2D, fc.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, typeVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		float nextAdvance = (fc.advance >> 6) * scale;
		x += nextAdvance;
		totalWidth += nextAdvance;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	return totalWidth;
}

Window::Window(int width, int height, const char* name, ResourceManager& manager) : manager(manager)
{
	// Attempt to initialize GLFW
	if (!glfwInit())
		throw std::exception("Failed to initialize GLFW");

	// Set min/max version of GLFW to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core profile (as opposed to immediate)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->width = width;
	this->height = height;
	this->playabeWidth = 0;
	this->playableHeight = 0;
	this->name = name;
	this->window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (!window)
		throw std::exception("Failed to initialize window");

	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::exception("Failed to initialize GLAD");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetErrorCallback(error_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Window::~Window()
{
	// Free remaining memory and terminate GLFW
	glfwTerminate();
	std::cout << "glfw terminated";
}

bool Window::beginRendering(Controller controller, float playableWidth, float playableHeight, float offsetWidth, float offsetHeight)
{
	float timeSinceLastFrame = 0.0f;
	float timeOfLastFrame = 0.0f;
	float currentTime = 0.0f;
	controller.init();
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		if (controller.ended()) break;
		controller.processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = glfwGetTime();
		timeSinceLastFrame = currentTime - timeOfLastFrame;
		timeOfLastFrame = currentTime;

		const std::unordered_map<int, GameObject>& objects = controller.updateObjects(timeSinceLastFrame);

		drawUI(playableWidth, playableHeight, offsetWidth, offsetHeight, controller.paused(), controller.playerHealth(), controller.activeEnemyHealth(), controller.score());

		for (const auto& object : objects)
		{
			draw(object.second.getSpriteName(), 
				object.second.getPosition(), 
				object.second.getSize(), 
				object.second.getRotation(), 
				object.second.getColor());
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return true;
}

void Window::initGame(int playableWidth, int playableHeight, std::string fontName, int fontRenderSize)
{
	this->playabeWidth = playableWidth;
	this->playableHeight = playableHeight;
	this->fontName = fontName;
	this->fontRenderSize = fontRenderSize;

	initShaders();
	initVertexData();
	initFreeType(fontName, fontRenderSize);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}