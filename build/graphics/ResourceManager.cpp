#include <fstream>
#include <sstream>
#include <iostream>

#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ResourceManager::~ResourceManager()
{
	for (auto it = shaders.begin(); it != shaders.end(); it++)
	{
		glDeleteProgram(it->second.getID());
	}
	for (auto it = sprites.begin(); it != sprites.end(); it++)
	{
		glDeleteTextures(1, &it->second.getID());
	}
}

void ResourceManager::initialize(std::string infoFile, std::string textureBasePath)
{
	std::cout << __func__ << std::endl;
	std::string infoFileFullPath = textureBasePath + infoFile;
	std::ifstream inputStream(infoFileFullPath);
	std::string lineFromStream;
	if (!inputStream) throw std::exception("Failed to load texture information!");
	while (std::getline(inputStream, lineFromStream))
	{
		std::stringstream ss(lineFromStream);
		std::string textureName;
		ss >> textureName;
		std::string textureFileName;
		ss >> textureFileName;
		bool useAlpha;
		ss >> useAlpha;

		std::string fullTextureFilePath = textureBasePath + textureFileName;

		generateSprite(textureName, fullTextureFilePath.c_str(), useAlpha);
	}
}

void ResourceManager::generateShader(std::string shaderName, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	if (geometryPath != nullptr && sizeof(geometryPath) == 0)
	{
		geometryPath = nullptr;
	}
	std::ifstream vertexInput = std::ifstream(vertexPath);
	std::ifstream fragmentInput = std::ifstream(fragmentPath);
	std::ifstream geometryInput;
	if (geometryPath != nullptr) geometryInput = std::ifstream(geometryPath);

	if (!vertexInput || !fragmentInput || (geometryPath != nullptr && !geometryInput))
	{
		std::string errorStr = "An error occured while generating a shader (" + shaderName + ").";
		throw std::exception(errorStr.c_str());
	}

	const char* vertexSource;
	const char* fragmentSource;
	const char* geometrySource;

	std::stringstream vertexSS, fragmentSS, geometrySS;

	vertexSS << vertexInput.rdbuf();
	fragmentSS << fragmentInput.rdbuf();
	if (geometryPath != nullptr) geometrySS << geometryInput.rdbuf();

	vertexInput.close();
	fragmentInput.close();
	if (geometryPath != nullptr) geometryInput.close();

	std::string vertexSourceStr = vertexSS.str();
	std::string fragSourceStr = fragmentSS.str();
	std::string geometrySourceStr;
	if (geometryPath != nullptr) geometrySourceStr = geometrySS.str();

	Shader shade(vertexSourceStr.c_str(), fragSourceStr.c_str(), geometrySourceStr.c_str());

	shaders.emplace(std::make_pair(shaderName, shade));
}

void ResourceManager::generateShader(std::string shaderName, bool useGeometry)
{
	std::string vertexPath = "./shaders/" + shaderName + ".vert";
	std::string fragmentPath = "./shaders/" + shaderName + ".frag";
	std::string geometryPath = "./shaders/" + shaderName + ".geo";
	if(useGeometry)
		return generateShader(shaderName, vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str());
	else
		return generateShader(shaderName, vertexPath.c_str(), fragmentPath.c_str(), nullptr);
}

void ResourceManager::generateSprite(std::string name, const char* spritePath, bool useAlpha)
{
	int width, height, numChannels;
	int internalFormat, imageFormat;
	if (useAlpha)
	{
		internalFormat = GL_RGBA;
		imageFormat = GL_RGBA;
	}
	else
	{
		internalFormat = GL_RGB;
		imageFormat = GL_RGB;
	}
	unsigned char* imageData = stbi_load(spritePath, &width, &height, &numChannels, 0);
	if (imageData == nullptr)
	{
		std::cout << "Failed to load image (" << name << ")";
		throw std::exception("Failed to load image data");
	}
	Sprite generatedSprite(name, width, height, internalFormat, imageFormat);
	try
	{
		generatedSprite.loadTexture(imageData);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	stbi_image_free(imageData);

	sprites.emplace(std::make_pair(name, generatedSprite));
}

Shader& ResourceManager::getShader(std::string shader)
{
	return shaders.at(shader);
}

Sprite& ResourceManager::getSprite(std::string sprite)
{
	return sprites.at(sprite);
}

