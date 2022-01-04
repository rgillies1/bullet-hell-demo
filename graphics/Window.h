#ifndef BULLET_WINDOW_H
#define BULLET_WINDOW_H

#include <glad/glad.h>
#include <glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../state/Game.h"
#include "Shader.h"
#include "../state/Controller.h"
#include "ResourceManager.h"

struct FaceCharacter
{
	unsigned int textureID, advance;
	glm::ivec2 size, bearing;
	FaceCharacter(unsigned int textureID, unsigned int advance, glm::ivec2 size, glm::ivec2 bearing)
	{
		this->textureID = textureID;
		this->advance = advance;
		this->size = size;
		this->bearing = bearing;
	}
};

class Window
{
private:
	ResourceManager& manager;
	GLFWwindow* window;
	FT_Library FTLib;
	FT_Face currentFace;
	int width, height, playabeWidth, playableHeight, fontRenderSize;
	int playerInfoX, playerInfoY, enemyInfoX, enemyInfoY;
	unsigned int VAO, VBO, EBO;
	unsigned int typeVAO, typeVBO, typeEBO;
	const char* name;
	std::string fontName;

	std::unordered_map<char, FaceCharacter> faceCharacters;

	void initVertexData();
	void initShaders();
	void initFreeType(std::string fontName, int renderSize);
	void draw(std::string spriteName, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color);
	void drawUI(float playableWidth, float playableHeight, float offsetWidth, float offsetHeight, 
		bool gameIsPaused, int playerHealth, int enemyHealth, int score);
	float renderText(std::string toRender, float x, float y, float scale, glm::vec3 color);
public:
	Window(int width, int height, const char* name, ResourceManager& manager);
	~Window();
	bool beginRendering(Controller controller, float playableWidth, float playableHeight, float offsetWidth, float offsetHeight);
	void initGame(int playableWidth, int playableHeight, std::string fontName, int fontRenderSize);
};

#endif