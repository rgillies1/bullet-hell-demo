
#ifndef GAME_SPRITE_H
#define GAME_SPRITE_H

#include <string>

#include <glad/glad.h>

class Sprite
{
private:
	std::string name;
	unsigned int textureID;
	int width;
	int height;
	int internalFormat;
	int imageFormat;
	int sWrap;
	int tWrap;
	int maxFilter;
	int minFilter;
public:
	Sprite(std::string name, int width, int height, int internalFormat = GL_RGB, int imageFormat = GL_RGB,
		int sWrap = GL_REPEAT, int tWrap = GL_REPEAT, int maxFilter = GL_LINEAR, int minFilter = GL_LINEAR);
	~Sprite();

	std::string		getName();
	unsigned int&	getID();
	int				getWidth();
	int				getHeight();

	void setWidth(int width);
	void setHeight(int width);

	void loadTexture(unsigned char* textureData);
	void bind();
};

#endif