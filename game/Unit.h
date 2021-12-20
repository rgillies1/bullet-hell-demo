#ifndef UNIT_H
#define UNIT_H

#include <string>

#include "GameObject.h"
#include "../graphics/Sprite.h"

class Unit : public GameObject
{
private:
	float health;
public:
	Unit(std::string name, int id, Sprite sprite, float posX, float posY, float sizeX, float sizeY,
		float rotation, float health,
		float velocityX = 0.0f, float velocityY = 0.0f, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);

	Unit(std::string, int id, Sprite sprite, float health);
};

#endif