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
	Sprite playerSprite = manager.getSprite("player");
	Sprite targetSprite = manager.getSprite("Star");
	Unit player("player", 0, playerSprite, 0, 0, 140, 140, 0.0f, 1);
	player.setMinFireDelay(0.07f);

	player.setUpdateFunction(
		[](GameObject* obj, float timeSinceLastFrame) 
		{
			obj->setVelocity(glm::vec2(0, 0));
		}
	);
	player.setHitboxCircle(player.getPosition().x, player.getPosition().y, player.getSize().x / 2);
	
	std::vector<Polygon> starData{
		Polygon(std::vector<glm::vec2> { glm::vec2(70, 0), glm::vec2(48.73, 46.09), glm::vec2(91.63, 46.09) }),
		Polygon(std::vector<glm::vec2> { glm::vec2(140, 53.48), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) }),
		Polygon(std::vector<glm::vec2> { glm::vec2(113.26, 140), glm::vec2(70, 116.09), glm::vec2(105, 89.35) }),
		Polygon(std::vector<glm::vec2> { glm::vec2(26.74, 140), glm::vec2(35, 89.35), glm::vec2(70, 116.09) }),
		Polygon(std::vector<glm::vec2> { glm::vec2(0, 53.48), glm::vec2(48.37, 46.09), glm::vec2(35, 89.35) }),
		Polygon(std::vector<glm::vec2> { glm::vec2(70, 116.09), glm::vec2(35, 89.35), glm::vec2(48.37, 46.09), glm::vec2(91.63, 46.09), glm::vec2(105, 89.35) })
	};
	//player.setHitboxCompositePolygon(starData);

	Unit target("target", 1, targetSprite, 300, 400, 140, 140, 0.0f, 1);
	glm::vec2 tarPos = target.getPosition();
	std::vector<Polygon> tarData{
		Polygon(std::vector<glm::vec2> { glm::vec2(70, 0) + tarPos, glm::vec2(48.73, 46.09) + tarPos, glm::vec2(91.63, 46.09) + tarPos }),
		Polygon(std::vector<glm::vec2> { glm::vec2(140, 53.48) + tarPos, glm::vec2(91.63, 46.09) + tarPos, glm::vec2(105, 89.35) + tarPos }),
		Polygon(std::vector<glm::vec2> { glm::vec2(113.26, 140) + tarPos, glm::vec2(70, 116.09) + tarPos, glm::vec2(105, 89.35) + tarPos }),
		Polygon(std::vector<glm::vec2> { glm::vec2(26.74, 140) + tarPos, glm::vec2(35, 89.35) + tarPos, glm::vec2(70, 116.09) + tarPos }),
		Polygon(std::vector<glm::vec2> { glm::vec2(0, 53.48) + tarPos, glm::vec2(48.37, 46.09) + tarPos, glm::vec2(35, 89.35) + tarPos }),
		Polygon(std::vector<glm::vec2> { glm::vec2(70, 116.09) + tarPos, glm::vec2(35, 89.35) + tarPos, glm::vec2(48.37, 46.09) + tarPos, glm::vec2(91.63, 46.09) + tarPos, glm::vec2(105, 89.35) + tarPos })
	};

	target.setHitboxCompositePolygon(tarData);
	//target.setHitboxCircle(target.getPosition().x, target.getPosition().y, target.getSize().x / 2);
	objects.push_back(player);


	target.setUpdateFunction(
		[](GameObject* obj, float timeSinceLastFrame) 
		{
			std::function<void(GameObject*, float)> bullet_func = 
				[](GameObject* obj, float timeSinceLastFrame) 
				{ 
					std::random_device rd;
					std::uniform_int_distribution<int> dist(0, 3);
					int dir = dist(rd);
					int vel = std::max(abs(obj->getVelocity().x), abs(obj->getVelocity().y));
					glm::vec2 newVel(0, 0);
					switch (dir)
					{
					case 0:
						newVel = glm::vec2(0, vel);
						break;
					case 1:
						newVel = glm::vec2(0, -vel);
						break;
					case 2:
						newVel = glm::vec2(vel, 0);
						break;
					case 3:
						newVel = glm::vec2(-vel, 0);
						break;
					}
					obj->setVelocity(newVel);
				};
			BulletInfo info = BulletInfo(BulletType::CIRCULAR, Direction::NORTH, glm::vec2(20, 20), glm::vec2(0, -300), glm::vec4(1, 1, 1, 1), 0.0f);
			std::pair<BulletInfo, std::function<void(GameObject*, float)>> bulletPair = std::make_pair(info, bullet_func);
			obj->getBulletsToFire().push(bulletPair);
		});
	objects.push_back(target);
	//for (int i = 0; i < 100; i++)
	//{
	//	std::string NAME = "TEST" + std::to_string(i);
	//	Unit* test = new Unit(NAME, 200 + i, manager.getSprite("circle"), 0 + (5*i), 0 + (10 * i), 100, 100, 0.0f, 1);
	//	test->setHitboxCircle(target.getPosition().x, target.getPosition().y, target.getSize().x / 2);
	//	test->setVelocity(glm::vec2(100, 0));
	//	//std::cout << test.getName() << std::endl;
	//	objects.push_back(*test);
	//}

	//{
	//	Unit* t1 = new Unit("t1", 3000, manager.getSprite("circle"), 200, 200, 100, 100, 0.0f, 1);
	//	t1->setVelocity(glm::vec2(50, 0));		
	//	Unit* t2 = new Unit("t2", 4000, manager.getSprite("square"), 600, 600, 100, 100, 0.0f, 1);
	//	t2->setVelocity(glm::vec2(0, -50));
	//	objects.push_back(*t1);
	//	objects.push_back(*t2);
	//}
}

void Game::addObject(GameObject& toAdd)
{
	objects.push_back(toAdd);
}

void Game::fireNormalBullet(GameObject& from, BulletType type, Direction direction, glm::vec2 size, glm::vec2 velocity,
	glm::vec4 color, float rotation)
{
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
