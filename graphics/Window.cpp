#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <random>

#include "Window.h"


const int DEFAULT_BUFFER_SIZE = 1200;

const char* vertexPath = "graphics/shaders/vertex.vert";
const char* fragmentPath = "graphics/shaders/fragment.frag";

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

	// Shape buffer
	glGenVertexArrays(1, &shapeVAO);
	glGenBuffers(1, &shapeVBO);
	glGenBuffers(1, &shapeEBO);

	glBindVertexArray(shapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferData(GL_ARRAY_BUFFER, 2048*sizeof(float), NULL, GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2048 * sizeof(int), NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	manager.generateShader("shape", "./graphics/shaders/shape.vert", "./graphics/shaders/shape.frag");
}

void Window::draw(Sprite toRender, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color)
{
	Shader shader = manager.getShader("sprite");
	shader.use();
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

void Window::drawTriangle(float vertices[], int numVertices, int polygonMode)
{
	Shader shader = manager.getShader("shape");
	shader.use();
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	glBindVertexArray(this->shapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices*sizeof(float), vertices);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::drawRectangle(float vertices[], int numVertices, int indices[], int numIndices, int polygonMode)
{
	Shader shader = manager.getShader("shape");
	shader.use();
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	glBindVertexArray(this->shapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices*sizeof(float), vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shapeEBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, numIndices*sizeof(int), indices);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::drawCircle(float center, float radius, int steps)
{
	Shader shader = manager.getShader("shape");
	shader.use();
	float vertices[1080] = {0};
	float bufferSize = 3.0f * (float)steps * sizeof(float);
	for (int i = 0; i < steps; i++)
	{
		float theta = (M_PI * 2.0f * (float)i) / ((float)steps);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		//std::cout << x << " : " << y << std::endl;
		vertices[i * 3] = center + x;
		vertices[i * 3 + 1] = center + y;
	}
	glBindVertexArray(this->shapeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, vertices);

	glDrawArrays(GL_LINE_LOOP, 0, 3*steps);
	glBindVertexArray(0);
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

	initShaders();
	initVertexData();

}

Window::~Window()
{
	// Free remaining memory and terminate GLFW
	glfwTerminate();
	std::cout << "glfw terminated";
}

bool Window::beginRendering(Controller controller)
{
	float timeSinceLastFrame = 0.0f;
	float timeOfLastFrame = 0.0f;
	float currentTime = 0.0f;
	controller.init();
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		controller.processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = glfwGetTime();
		timeSinceLastFrame = currentTime - timeOfLastFrame;
		timeOfLastFrame = currentTime;

		const std::unordered_map<int, GameObject>& objects = controller.updateObjects(timeSinceLastFrame);

		for (const auto& object : objects)
		{
			draw(object.second.getSprite(), 
				object.second.getPosition(), 
				object.second.getSize(), 
				object.second.getRotation(), 
				object.second.getColor());
		}

		//controller.clean();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return true;
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