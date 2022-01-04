#include "Game.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <random>
#include <iostream>
#include <algorithm>

// HACK: some recent change now causes the game to segfault after the player dies

Game::Game(float playableWidth, float playableHeight, float widthOffset, float heightOffset, ResourceManager& manager)
	: manager(manager)
{
	this->playableWidth = playableWidth;
	this->playableHeight = playableHeight;
	this->widthOffset = widthOffset;
	this->heightOffset = heightOffset;
	paused = false;
	isEnded = false;
	player = nullptr;
	activeEnemy = nullptr;
}

Game::~Game()
{

}

void Game::init()
{
	keyStrokes.emplace(Binding::PLAYER_MOVE_UP, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_DOWN, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_LEFT, false);
	keyStrokes.emplace(Binding::PLAYER_MOVE_RIGHT, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_UP, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_DOWN, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_LEFT, false);
	keyStrokes.emplace(Binding::PLAYER_FIRE_RIGHT, false);

	properties.emplace("player_move_speed", "500.0");
	properties.emplace("player_bullet_speed", "2000.0");
	properties.emplace("player_bullet_size", "10.0");
	properties.emplace("player_bullet_sprite_name", "solid-square");
}

void Game::addObject(GameObject& toAdd)
{
	std::cout << __func__ << ": toAdd ID is " << toAdd.getID() << std::endl;
	objects.emplace(toAdd.getID(), toAdd);
	if (objects.size() == 1) player = &objects.at(0);
	else if (activeEnemy == nullptr) activeEnemy = &objects.at(toAdd.getID());
}

void Game::clean()
{
	if (!player->isAlive()) isEnded = true;
	auto it = objects.begin();
	while (it != objects.end())
	{
		if (it->second.isAlive())
		{
			it++;
		}
		else
		{
			int objScore = it->second.getScore();
			it = objects.erase(it);
			score += objScore;
		}
	}
}

void Game::adjustPosition(GameObject& obj)
{
	float objX = obj.getPosition().x;
	float objY = obj.getPosition().y;

	float left_limit = widthOffset;
	float right_limit = playableWidth + widthOffset;
	float upper_limit = heightOffset;
	float lower_limit = playableHeight + heightOffset;
	if (objX + obj.getSize().x > right_limit || objX < left_limit
		|| objY + obj.getSize().y > lower_limit || objY < upper_limit)
	{
		if (obj.getID() >= bulletIDBase)
		{
			obj.kill();
		}
		else
		{
			float newX = objX, newY = objY;

			if (objX + obj.getSize().x > right_limit)
				newX = right_limit - obj.getSize().x;
			if (objX < left_limit) newX = left_limit;
			if (objY + obj.getSize().y > lower_limit)
				newY = lower_limit - obj.getSize().y;
			if (objY < upper_limit) newY = upper_limit;
			
			obj.setPosition(glm::vec2(newX, newY));
		}
	}
}

void Game::processBullets(GameObject& obj)
{
	std::vector<GameObject>& bulletInfo = obj.getBulletsToFire();
	if (!bulletInfo.empty())
	{
		for (int index = 0; index < bulletInfo.size(); index++)
		{
			GameObject info = bulletInfo.at(bulletInfo.size() - 1);
			fireBullet(obj, info);
			bulletInfo.pop_back();
		}
	}
}

void Game::fireBullet(GameObject& from, GameObject& bullet)
{
	float currentTime = glfwGetTime();
	float timeSinceLastFire = currentTime - from.getTimeOfLastFire();
	if (timeSinceLastFire >= from.getMinFireDelay())
	{
		from.setTimeOfLastFire(currentTime);
		int bulletID = bulletIDBase + lifetimeBulletsFired++;
		bullet.setID(bulletID);
		bullet.setOriginID(from.getID());

		addObject(bullet);
	}
}

void Game::processKeystrokes()
{
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
		firePlayerBullet(Binding::PLAYER_FIRE_UP);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_DOWN] == true)
	{
		firePlayerBullet(Binding::PLAYER_FIRE_DOWN);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_LEFT] == true)
	{
		firePlayerBullet(Binding::PLAYER_FIRE_LEFT);
	}
	if (keyStrokes[Binding::PLAYER_FIRE_RIGHT] == true)
	{
		firePlayerBullet(Binding::PLAYER_FIRE_RIGHT);
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

void Game::firePlayerBullet(Binding forBinding)
{
	std::string name = "Player_bullet", spriteName = properties["player_bullet_sprite_name"];
	int id = 0, health = 1, damage = 1, score = 0;
	float pos_x = 0, pos_y = 0;
	float rotation = 0.0f;
	float size_x = atof(properties["player_bullet_size"].c_str()), size_y = size_x;
	float vel_x = 0.0f, vel_y = 0.0f;
	float accel_x = 0.0f, accel_y = 0.0f;
	float color_r = 0.0f, color_g = 255.0f;
	float color_b = 255.0f, color_a = 1.0f;
	float minFireDelay = 0.10f;
	std::function<void(GameObject*, float)> updateFunction = [](GameObject* obj, float) {};
	std::vector<float> hitboxInfo{ size_x / 2.0f };

	float bulletSpeed = atof(properties["player_bullet_speed"].c_str());

	switch (forBinding)
	{
	case Binding::PLAYER_FIRE_UP:
		pos_x = (player->getPosition().x + (player->getSize().x / 2)) - (size_x / 2);
		pos_y = (player->getPosition().y - size_y);
		vel_y = -bulletSpeed;
		player->setRotation(0.0f);
		break;
	case Binding::PLAYER_FIRE_DOWN:
		pos_x = (player->getPosition().x + (player->getSize().x / 2)) - (size_x / 2);
		pos_y = (player->getPosition().y + player->getSize().y);
		vel_y = bulletSpeed;
		player->setRotation(180.0f);
		break;
	case Binding::PLAYER_FIRE_LEFT:
		pos_x = player->getPosition().x - size_x;
		pos_y = (player->getPosition().y + (player->getSize().y / 2)) - (size_y / 2);
		vel_x = -bulletSpeed;
		player->setRotation(270.0f);
		break;
	case Binding::PLAYER_FIRE_RIGHT:
		pos_x = player->getPosition().x + player->getSize().x;
		pos_y = (player->getPosition().y + (player->getSize().y / 2)) - (size_y / 2);
		vel_x = bulletSpeed;
		player->setRotation(90.0f);
		break;
	}
	GameObject bullet(name, id, spriteName, health, damage, score,
		pos_x, pos_y, rotation, size_x, size_y, vel_x, vel_y, accel_x, accel_y,
		color_r, color_g, color_b, color_a);

	fireBullet(*player, bullet);
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