#include "Sprite.h"
#include <iostream>

Sprite::Sprite(std::string name, int width, int height, int internalFormat, 
	int imageFormat, int sWrap, int tWrap, int maxFilter, int minFilter)
{
	this->name = name;
	this->width = width;
	this->height = height;
	this->internalFormat = internalFormat;
	this->imageFormat = imageFormat;
	this->sWrap = sWrap;
	this->tWrap = tWrap;
	this->maxFilter = maxFilter;
	this->minFilter = minFilter;

	glGenTextures(1, &this->textureID);
}

Sprite::~Sprite()
{

}

std::string Sprite::getName()
{
	return name;
}

unsigned int& Sprite::getID()
{
	return textureID;
}

int Sprite::getWidth()
{
	return width;
}

void Sprite::setWidth(int width)
{
	this->width = width;
}

int Sprite::getHeight()
{
	return height;
}

void Sprite::setHeight(int width)
{
	this->height = height;
}

void Sprite::loadTexture(unsigned char* textureData)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}
