#include "Unit.h"

Unit::Unit(std::string name, int id, Sprite sprite, float posX, float posY, float sizeX, float sizeY,
	float rotation, float health,
	float velocityX, float velocityY, float red, float green, float blue, float alpha)
	: GameObject(name, id, sprite, posX, posY, rotation, sizeX, sizeY, velocityX, velocityY, red, green, blue, alpha)
{
	this->health = health;
}

Unit::Unit(std::string name, int id, Sprite sprite, float health)
	: GameObject(name, id, sprite, 1.0f, 1.0f, sprite.getWidth(), sprite.getHeight(), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
{
	this->health = health;
}
