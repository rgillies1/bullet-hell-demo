#include <map>

#include "Shader.h"
#include "Sprite.h"

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
class ResourceManager
{
private:
	std::map<std::string, Shader> shaders;
	std::map<std::string, Sprite> sprites;
public:
	~ResourceManager();
	void generateShader(std::string shaderName, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void generateShader(std::string shaderName, bool useGeometry);
	void generateSprite(std::string name, const char* spritePath, bool useAlpha);
	Shader& getShader(std::string shader);
	Sprite& getSprite(std::string sprite);
};

#endif