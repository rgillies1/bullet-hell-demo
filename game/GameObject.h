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

//enum class HitboxShape
//{
//	CIRCLE, POLYGON, COMPOSITE_POLYGON
//};
//
//struct GameObject
//{
//	std::string name, spriteName;
//	int id, health, damage, score;
//	float vel_x, vel_y, accel_x, accel_y, size_x, size_y, pos_x, pos_y, color_r, color_g, color_b, color_a, rotation, def_minFireDelay;
//	std::function<void(GameObject*, float)> updateFunction;
//	HitboxShape hitboxShape;
//	std::vector<float> hitboxInfo;
//	GameObject(std::string name,
//		int id, std::string spriteName,
//		int health, int damage, int score,
//		float pos_x, float pos_y,
//		float rotation, float size_x,
//		float size_y, float vel_x,
//		float vel_y , float accel_x,
//		float accel_y, float color_r,
//		float color_g, float color_b,
//		float color_a, float minFireDelay,
//		std::function<void(GameObject*, float)> updateFunction,
//		HitboxShape hitboxShape,
//		std::vector<float> hitboxInfo)
//	{
//		this->name = name;
//		this->id = id;
//		this->spriteName = spriteName;
//		this->damage = damage;
//		this->score = score;
//		this->health = health;
//		this->pos_x = pos_x;
//		this->pos_y = pos_y;
//		this->rotation = rotation;
//		this->size_x = size_x;
//		this->size_y = size_y;
//		this->vel_x = vel_x;
//		this->vel_y = vel_y;
//		this->accel_x = accel_x;
//		this->accel_y = accel_y;
//		this->color_r = color_r;
//		this->color_b = color_b;
//		this->color_g = color_g;
//		this->color_a = color_a;
//		this->def_minFireDelay = minFireDelay;
//		this->updateFunction = updateFunction;
//		this->hitboxShape = hitboxShape;
//		this->hitboxInfo = hitboxInfo;
//	}
//};

class GameObject
{
private:
	std::string	name, spriteName;
	int			id, originId;
	int			health, damage, score;

	float		rotation;
	float		timeOfLastFire;
	float		minFireDelay;
	bool		alive;
	glm::vec2	position, size, velocity, acceleration;
	glm::vec4	color;
	//Sprite		sprite;

	std::function<void(GameObject*, float)> updateFunction;
	std::vector<GameObject> toFire;

	std::shared_ptr<Shape> hitbox;
	void buildDefaultHitbox();
public:
	GameObject(std::string name, int id, std::string spriteName, int health, int damage, int score, glm::vec2 position, float rotation, glm::vec2 size, 
		glm::vec2 velocity, glm::vec2 acceleration, glm::vec4 color);
	GameObject(std::string name, int id, std::string spriteName, int health, int damage, int score, float posX, float posY, float rotation, float sizeX, float sizeY,
		float velocityX, float velocityY, float accelerationX, float accelerationY, float red, float green, float blue, float alpha);
	~GameObject();

	bool					isAlive() const;
	std::string				getName() const;
	int						getID() const;
	int						getOriginID() const;
	int						getHealth() const;
	int						getScore() const;
	int						getDamage() const;
	std::string				getSpriteName() const;
	glm::vec2				getPosition() const;
	glm::vec2				getSize() const;
	glm::vec2				getVelocity() const;
	glm::vec2				getAcceleration() const;
	glm::vec4				getColor() const;
	float					getRotation() const;
	float					getTimeOfLastFire() const;
	float					getMinFireDelay() const;
	std::shared_ptr<Shape>& getHitbox();
	std::vector<GameObject>&	getBulletsToFire();

	void setID(int id);
	void setOriginID(int originId);
	void setSpriteName(std::string spriteName);
	void setPosition(glm::vec2 position);
	void setPosition(float posX, float posY);
	void setSize(glm::vec2 size);
	void setSize(float sizeX, float sizeY);
	void setVelocity(glm::vec2 velocity);
	void setVelocity(float velocityX, float velocityY);
	void setAcceleration(glm::vec2 acceleration);
	void setAcceleration(float accelerationX, float accelerationY);
	void setColor(glm::vec4 color);
	void setColor(float red, float green, float blue, float alpha);
	void setRotation(float rotation);
	void setUpdateFunction(std::function<void(GameObject*, float)> func);
	void setTimeOfLastFire(float timeSinceLastFire);
	void setMinFireDelay(float minFireDelay);
	void setHitboxPolygon(std::vector<glm::vec2> vertices);
	void setHitboxCompositePolygon(std::vector<Polygon> shapes);
	void setHitboxCircle(float x, float y, float radius);
	void hurt(int damage);
	void heal(int amount);
	void kill();
	void fire(GameObject toFire);
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