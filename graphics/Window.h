#ifndef BULLET_WINDOW_H
#define BULLET_WINDOW_H

#include <glad/glad.h>
#include <glfw3.h>
#include "../state/Game.h"
#include "Shader.h"
#include "../state/Controller.h"
#include "ResourceManager.h"

class Window
{
private:
	ResourceManager& manager;
	int width, height;
	unsigned int VAO, VBO, EBO;
	unsigned int shapeVAO, shapeVBO, shapeEBO;
	const char* name;
	GLFWwindow* window;
	void initVertexData();
	void initShaders();
	void draw(Sprite toRender, glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color);
	void drawTriangle(float vertices[], int numVertices, int polygonMode);
	void drawRectangle(float vertices[], int numVertices, int indices[], int numIndices, int polygonMode);
	void drawCircle(float center, float radius, int steps);
public:
	Window(int width, int height, const char* name, ResourceManager& manager);
	~Window();
	bool beginRendering(Controller controller);
};

#endif