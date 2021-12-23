#include "Game.h"
#include "../game/Unit.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <iostream>
#include <algorithm>

Game::Game(float playableWidth, float playableHeight, ResourceManager& manager) 
	: manager(manager)
{
	this->playableWidth = playableWidth;
	this->playableHeight = playableHeight;
	paused = false;
}

Game::~Game()
{

}

void Game::init()
{
	manager.generateSprite("player", "./graphics/textures/player.png", true);
	manager.generateSprite("square", "./graphics/textures/square.png", true);
	manager.generateSprite("diamond", "./graphics/textures/diamond.png", true);
	manager.generateSprite("circle", "./graphics/textures/circle.png", true);
	manager.generateSprite("Star", "./graphics/textures/star.png", true);


}

void Game::addObject(GameObject& toAdd)
{
	objects.push_back(toAdd);
}

void Game::addObject(ObjectInfo& info)
{
	GameObject toAdd(info.name, info.id, manager.getSprite(info.spriteName),
		info.pos_x, info.pos_y, info.rotation, info.size_x, info.size_y,
		info.vel_x, info.vel_y, info.color_r, info.color_b, info.color_g,
		info.color_a);
	toAdd.setUpdateFunction(info.updateFunction);
	toAdd.setMinFireDelay(info.def_minFireDelay);
	switch (info.hitboxShape)
	{
	case HitboxShape::CIRCLE:
		toAdd.setHitboxCircle(toAdd.getPosition().x, toAdd.getPosition().y, info.hitboxInfo[0]);
		break;
	case HitboxShape::POLYGON:
	{
		std::vector<glm::vec2> coords;
		for (int i = 0; i < info.hitboxInfo.size() - 1; i += 2)
		{
			float x_coord = info.hitboxInfo[i];
			float y_coord = info.hitboxInfo[i + 1];
			glm::vec2 coord(x_coord, y_coord);
			coords.push_back(coord);
		}
		toAdd.setHitboxPolygon(coords);
	}
		break;
	case HitboxShape::COMPOSITE_POLYGON:
	{
		std::vector<Polygon> polys;
		std::vector<glm::vec2> polyHitboxes;
		for (int i = 0; i < info.hitboxInfo.size() - 1; i += 2)
		{
			float x_coord = info.hitboxInfo[i];
			float y_coord = info.hitboxInfo[i + 1];
			if (x_coord == y_coord == -1.0f)
			{
				polys.push_back(Polygon(polyHitboxes));
				polyHitboxes.clear();
			}
			glm::vec2 coord(x_coord, y_coord);
			polyHitboxes.push_back(coord);
		}
		toAdd.setHitboxCompositePolygon(polys);
	}
		break;
	}

	objects.push_back(toAdd);
}

void Game::fireNormalBullet(GameObject& from, BulletType type, Direction direction, glm::vec2 size, glm::vec2 velocity,
	glm::vec4 color, float rotation)
{
	float time = glfwGetTime() - from.getTimeOfLastFire();
	if (abs(time) < from.getMinFireDelay()) return;
	else from.setTimeOfLastFire(glfwGetTime());
	float x = 0, y = 0;
	switch (direction)
	{
	case Direction::NORTH:
		x = from.getPosition().x + (from.getSize().x / 2);
		x -= (size.x / 2);
		y = from.getPosition().y;
		y -= size.y;
		break;
	case Direction::EAST:
		x = from.getPosition().x;
		x -= size.x;
		y = from.getPosition().y + (from.getSize().y / 2);
		y -= (size.y / 2);
		break;
	case Direction::SOUTH:
		x = from.getPosition().x + (from.getSize().x / 2);
		x -= (size.x / 2);
		y = from.getPosition().y + from.getSize().y;
		break;
	case Direction::WEST:
		x = from.getPosition().x + from.getSize().x;
		y = from.getPosition().y + (from.getSize().y / 2);
		y -= (size.y / 2);
		break;
	}
	glm::vec2 bulletPosition(x, y);

	Sprite& texture = manager.getSprite("player");
	switch (type)
	{
	case BulletType::CIRCULAR:
		texture = manager.getSprite("circle");
		
		break;
	case BulletType::RECTANGULAR:
		break;
	case BulletType::TRIANGULAR:
		break;
	}

	GameObject& bullet = fireBullet(from, texture, bulletPosition, size, velocity, color, rotation, [](GameObject*, float) {});

	switch (type)
	{
	case BulletType::CIRCULAR:
		bullet.setHitboxCircle(bulletPosition.x, bulletPosition.y, bullet.getSize().x / 2);
		break;
	case BulletType::RECTANGULAR:
		break;
	case BulletType::TRIANGULAR:
		break;
	}
}

void Game::fireNormalBullet(GameObject& from, BulletInfo info, std::function<void(GameObject*, float)> bulletFunction)
{
	fireNormalBullet(from, info.type, info.direction, info.size, info.velocity, info.color, info.rotation);
	temp[temp.size() - 1].setUpdateFunction(bulletFunction);
}

void Game::adjustPosition(GameObject& obj)
{
	float objX = obj.getPosition().x;
	float objY = obj.getPosition().y;
	if (objX + obj.getSize().x > playableWidth || objX < 0.0f
		|| objY + obj.getSize().y > playableHeight || objY < 0.0f)
	{
		if (obj.getID() >= bulletIDBase)
		{
			if (objX > playableWidth || objX + obj.getSize().x < 0.0f
				|| objY > playableHeight || objY + obj.getSize().y < 0.0f)
			obj.kill();
		}
		else
		{
			float newX = objX, newY = objY;

			if (objX + obj.getSize().x > playableWidth)
				newX = playableWidth - obj.getSize().x;
			if (objX < 0.0f) newX = 0.0f;
			if (objY + obj.getSize().y > playableHeight)
				newY = playableHeight - obj.getSize().y;
			if (objY < 0.0f) newY = 0.0f;

			obj.setPosition(glm::vec2(newX, newY));
		}
	}
}

GameObject& Game::fireBullet(GameObject& from, Sprite texture, glm::vec2 position, glm::vec2 size,
	glm::vec2 velocity, glm::vec4 color, float rotation, std::function<void(GameObject*, float)> bulletfunc)
{
	std::string name = from.getName() + "_bullet";
	int id = bulletIDBase + (lifetimeBulletsFired++);
	GameObject* bullet = new GameObject(name, id, texture, position, rotation, size, velocity, color);
	bullet->setUpdateFunction(bulletfunc);
	bullet->setOriginID(from.getID());
	temp.push_back(*bullet);
	return temp[temp.size() - 1];
}

void Game::processBullets(GameObject& obj)
{
	std::queue<std::pair<BulletInfo, std::function<void(GameObject*, float)>>>& bulletInfo = obj.getBulletsToFire();
	if (!bulletInfo.empty())
	{
		for (int index = 0; index < bulletInfo.size(); index++)
		{
			BulletInfo info = bulletInfo.front().first;
			std::function<void(GameObject*, float)> bulletFunction = bulletInfo.front().second;
			bulletInfo.pop();
			fireNormalBullet(obj, info, bulletFunction);
		}
	}
}

std::vector<GameObject>& Game::getObjects()
{
	return objects;
}

void Game::updateObjects(float timeSinceLastUpdate)
{
	for (GameObject& obj : objects)
	{
		if (paused) return;
		glm::vec2 oldpos = obj.getPosition();
		obj.update(timeSinceLastUpdate);
		processBullets(obj);
		adjustPosition(obj);
	}
	for (GameObject bullet : temp)
	{
		objects.push_back(bullet);
	}
	temp.clear();
}

//void Game::setObjects(std::vector<GameObject>& objects)
//{
//	this->objects = objects;
//}
