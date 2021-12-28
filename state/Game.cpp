#include "Game.h"
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

	keyStrokes.emplace(Binding::PLAYER_MOVE_UP, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_DOWN, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_LEFT, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_RIGHT, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_UP, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_DOWN, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_LEFT, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_RIGHT, false);

	properties.emplace("player_move_speed", "500.0");
	properties.emplace("player_bullet_speed", "1000.0");
	properties.emplace("player_bullet_size", "20.0");
}

void Game::addObject(GameObject& toAdd)
{
	objects.emplace(toAdd.getID(), toAdd);
	if (objects.size() == 1) player = &objects.at(0);
}

void Game::addObject(ObjectInfo& info)
{
	GameObject toAdd(info.name, info.id, manager.getSprite(info.spriteName),
		info.pos_x, info.pos_y, info.rotation, info.size_x, info.size_y,
		info.vel_x, info.vel_y, info.accel_x, info.accel_y, info.color_r, 
		info.color_b, info.color_g, info.color_a);
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

	addObject(toAdd);
}

void Game::clean()
{
	if (!player->isAlive()) exit(0);
	auto it = objects.begin();
	while (it != objects.end())
	{
		if (it->second.isAlive())
		{
			it++;
		}
		else
		{
			it = objects.erase(it);
		}
	}
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

void Game::processBullets(GameObject& obj)
{
	std::queue<ObjectInfo>& bulletInfo = obj.getBulletsToFire();
	if (!bulletInfo.empty())
	{
		for (int index = 0; index < bulletInfo.size(); index++)
		{
			ObjectInfo info = bulletInfo.front();
			bulletInfo.pop();
			fireBullet(obj, info);
		}
	}
}

void Game::fireBullet(GameObject& from, ObjectInfo bulletInfo)
{
	int bulletID = bulletIDBase + lifetimeBulletsFired++;
	GameObject bullet(bulletInfo.name, bulletID, manager.getSprite(bulletInfo.spriteName), bulletInfo.pos_x,
		bulletInfo.pos_y, bulletInfo.rotation, bulletInfo.size_x, bulletInfo.size_y, bulletInfo.vel_x,
		bulletInfo.vel_y, bulletInfo.accel_x, bulletInfo.accel_y, bulletInfo.color_r, bulletInfo.color_g,
		bulletInfo.color_b, bulletInfo.color_a);
	bullet.setMinFireDelay(bulletInfo.def_minFireDelay);
	bullet.setUpdateFunction(bulletInfo.updateFunction);
	bullet.setOriginID(from.getID());


	addObject(bullet);
}

void Game::processKeystrokes()
{
	std::string name = "Player_bullet", spriteName = "circle";
	int id = 0;
	float pos_x = 0, pos_y = 0;
	float rotation = 0.0f;
	float size_x = atof(properties["player_bullet_size"].c_str()), size_y = size_x;
	float vel_x = 0.0f, vel_y = 0.0f;
	float accel_x = 0.0f, accel_y = 0.0f;
	float color_r = 0.0f, color_g = 255.0f;
	float color_b = 255.0f, color_a = 1.0f;
	float minFireDelay = 0.10f;
	std::function<void(GameObject*, float)> updateFunction = [](GameObject* obj, float){};
	std::vector<float> hitboxInfo{ size_x / 2.0f };


	if (keyStrokes[Binding::PLAYER_MOVE_UP] == true)
	{
		float moveSpeed = atof(properties["player_move_speed"].c_str());
		player->setVelocity(player->getVelocity().x, -moveSpeed);
	}
	if (keyStrokes[Binding::PLAYER_MOVE_DOWN] == true)
	{
		float moveSpeed = atof(properties["player_move_speed"].c_str());
		player->setVelocity(player->getVelocity().x, moveSpeed);
	}
	if (keyStrokes[Binding::PLAYER_MOVE_LEFT] == true)
	{
		float moveSpeed = atof(properties["player_move_speed"].c_str());
		player->setVelocity(-moveSpeed, player->getVelocity().y);
	}
	if (keyStrokes[Binding::PLAYER_MOVE_RIGHT] == true)
	{
		float moveSpeed = atof(properties["player_move_speed"].c_str());
		player->setVelocity(moveSpeed, player->getVelocity().y);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_UP] == true)
	{
		float bulletSpeed = atof(properties["player_bullet_speed"].c_str());
		player->setRotation(0.0f);
		float time = glfwGetTime() - player->getTimeOfLastFire();
		if (abs(time) < player->getMinFireDelay()) return;
		else player->setTimeOfLastFire(glfwGetTime());

		pos_x = (player->getPosition().x + (player->getSize().x / 2)) - (size_x / 2);
		pos_y = (player->getPosition().y - size_y);
		vel_y = -bulletSpeed;

		ObjectInfo bullet(name, id, spriteName,
			pos_x, pos_y, rotation, size_x, size_y,
			vel_x, vel_y, accel_x, accel_y, color_r,
			color_g, color_b, color_a, minFireDelay,
			updateFunction, HitboxShape::CIRCLE,
			hitboxInfo);

		fireBullet(*player, bullet);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_DOWN] == true)
	{

		float bulletSpeed = atof(properties["player_bullet_speed"].c_str());
		player->setRotation(180.0f);
		float time = glfwGetTime() - player->getTimeOfLastFire();
		if (abs(time) < player->getMinFireDelay()) return;
		else player->setTimeOfLastFire(glfwGetTime());

		pos_x = (player->getPosition().x + (player->getSize().x / 2)) - (size_x / 2);
		pos_y = (player->getPosition().y + player->getSize().y);
		vel_y = bulletSpeed;

		ObjectInfo bullet(name, id, spriteName,
			pos_x, pos_y, rotation, size_x, size_y,
			vel_x, vel_y, accel_x, accel_y, color_r,
			color_g, color_b, color_a, minFireDelay,
			updateFunction, HitboxShape::CIRCLE,
			hitboxInfo);

		fireBullet(*player, bullet);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_LEFT] == true)
	{
		float bulletSpeed = atof(properties["player_bullet_speed"].c_str());
		player->setRotation(270.0f);
		float time = glfwGetTime() - player->getTimeOfLastFire();
		if (abs(time) < player->getMinFireDelay()) return;
		else player->setTimeOfLastFire(glfwGetTime());

		pos_x = player->getPosition().x - size_x;
		pos_y = (player->getPosition().y + (player->getSize().y / 2)) - (size_y / 2);
		vel_x = -bulletSpeed;

		ObjectInfo bullet(name, id, spriteName,
			pos_x, pos_y, rotation, size_x, size_y,
			vel_x, vel_y, accel_x, accel_y, color_r,
			color_g, color_b, color_a, minFireDelay,
			updateFunction, HitboxShape::CIRCLE,
			hitboxInfo);

		fireBullet(*player, bullet);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_RIGHT] == true)
	{
		float bulletSpeed = atof(properties["player_bullet_speed"].c_str());
		player->setRotation(90.0f);
		float time = glfwGetTime() - player->getTimeOfLastFire();
		if (abs(time) < player->getMinFireDelay()) return;
		else player->setTimeOfLastFire(glfwGetTime());

		pos_x = player->getPosition().x + player->getSize().x;
		pos_y = (player->getPosition().y + (player->getSize().y / 2)) - (size_y / 2);
		vel_x = bulletSpeed;

		ObjectInfo bullet(name, id, spriteName,
			pos_x, pos_y, rotation, size_x, size_y,
			vel_x, vel_y, accel_x, accel_y, color_r,
			color_g, color_b, color_a, minFireDelay,
			updateFunction, HitboxShape::CIRCLE,
			hitboxInfo);

		fireBullet(*player, bullet);
	}
	if (keyStrokes[Binding::PLAYER_SLOW] == true)
	{
		properties["player_move_speed"] = "100.0";
	}
	else
	{
		properties["player_move_speed"] = "500.0";
	}
}

std::unordered_map<int, GameObject>& Game::getObjects()
{
	return objects;
}

void Game::updateObjects(float timeSinceLastUpdate)
{
	processKeystrokes();
	for (auto& objPair : objects)
	{
		if (paused) return;
		objPair.second.update(timeSinceLastUpdate);
		processBullets(objPair.second);
		adjustPosition(objPair.second);
	}
	for (GameObject bullet : temp)
	{
		addObject(bullet);
	}
	temp.clear();
}

//void Game::setObjects(std::vector<GameObject>& objects)
//{
//	this->objects = objects;
//}
