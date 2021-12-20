#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>
#include <queue>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BulletType.h"
#include "../graphics/Sprite.h"
#include "./Shapes/Shape.h"
#include "Shapes/Polygon.h"
#include "Shapes/CompositePolygon.h"
#include "Shapes/Circle.h"

class GameObject
{
private:
	std::string	name;
	int			id, originId;

	float		rotation;
	float		timeOfLastFire;
	float		minFireDelay;
	bool		alive;
	glm::vec2	position, size, velocity;
	glm::vec4	color;
	Sprite		sprite;

	std::function<void(GameObject*, float)> updateFunction;
	//std::queue<BulletInfo> toFire;
	std::queue<std::pair<BulletInfo, std::function<void(GameObject*, float)>>> toFire;

	std::shared_ptr<Shape> hitbox;
	void buildDefaultHitbox();
public:
	GameObject(std::string name, int id, Sprite sprite, glm::vec2 position, float rotation, glm::vec2 size, glm::vec2 velocity, glm::vec4 color);
	GameObject(std::string name, int id, Sprite sprite, float posX, float posY, float rotation, float sizeX, float sizeY,
		float velocityX, float velocityY, float red, float green, float blue, float alpha);

	bool					isAlive() const;
	std::string				getName() const;
	int						getID() const;
	int						getOriginID() const;
	Sprite					getSprite() const;
	glm::vec2				getPosition() const;
	glm::vec2				getSize() const;
	glm::vec2				getVelocity() const;
	glm::vec4				getColor() const;
	float					getRotation() const;
	float					getTimeOfLastFire() const;
	float					getMinFireDelay() const;
	std::shared_ptr<Shape>& getHitbox();
	std::queue<std::pair<BulletInfo, std::function<void(GameObject*, float)>>>&		
							getBulletsToFire();

	void setOriginID(int originId);
	void setSprite(Sprite sprite);
	void setPosition(glm::vec2 position);
	void setPosition(float posX, float posY);
	void setSize(glm::vec2 size);
	void setSize(float sizeX, float sizeY);
	void setVelocity(glm::vec2 velocity);
	void setVelocity(float velocityX, float velocityY);
	void setColor(glm::vec4 color);
	void setColor(float red, float green, float blue, float alpha);
	void setRotation(float rotation);
	void setUpdateFunction(std::function<void(GameObject*, float)> func);
	void setTimeOfLastFire(float timeSinceLastFire);
	void setMinFireDelay(float minFireDelay);
	void setHitboxPolygon(std::vector<glm::vec2> vertices);
	void setHitboxCompositePolygon(std::vector<Polygon> shapes);
	void setHitboxCircle(float x, float y, float radius);
	void kill();
	void update(float timeSinceLastFrame);

	bool operator==(const GameObject& rhs)
	{
		return this->getID() == rhs.getID();
	}
	bool operator!=(const GameObject& rhs)
	{
		return !(*this == rhs);
	}
	friend bool operator==(const GameObject& lhs, const GameObject& rhs)
	{
		return lhs.getID() == rhs.getID();
	}
	friend bool operator!=(const GameObject& lhs, const GameObject& rhs)
	{
		return !(lhs.getID() == rhs.getID());
	}
};

#endif