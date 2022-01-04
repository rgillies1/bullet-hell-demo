#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <unordered_map>
#include "../game/GameObject.h"
#include "../graphics/ResourceManager.h"
#include <glm/gtx/hash.hpp>
#include "../game/BulletType.h"
#include "../scripts/Script.h"
#include "Bindings.h"

class Game
{
private:
	float playableWidth, playableHeight, widthOffset, heightOffset;
	bool paused, isEnded;
	ResourceManager& manager;

	std::unordered_map<int, GameObject> objects;
	std::vector<GameObject> temp;

	GameObject* player, *activeEnemy;

	std::unordered_map<Binding, bool> keyStrokes;
	std::unordered_map<std::string, std::string> properties;

	int lifetimeBulletsFired = 0, score = 0;
	const int bulletIDBase = 90000;

	void adjustPosition(GameObject& obj);
	void processBullets(GameObject& obj);
	void fireBullet(GameObject& from, GameObject& bulletInfo);

	void processKeystrokes();
	void firePlayerBullet(Binding forBinding);
public:
	Game(float playableWidth, float playableHeight, float widthOffset, float heightOffset, ResourceManager& manager);
	~Game();
	std::unordered_map<int, GameObject>& getObjects();
	void init();
	void updateObjects(float timeSinceLastUpdate);
	void addObject(GameObject& toAdd);
	//void addObject(GameObject& info);
	void clean();
	bool ended()
	{
		return isEnded;
	}
	int getBulletIdBase()
	{
		return bulletIDBase;
	}
	void togglePause() 
	{ 
		if (paused) paused = false;
		else paused = true;
	};
	bool isPaused() 
	{
		return paused;
	};
	bool objectWithIDIsAlive(int id)
	{
		try
		{
			return objects.at(id).isAlive();
		}
		catch (std::exception)
		{
			return false;
		}
	}
	void killObjectById(int id)
	{
		objects.at(id).kill();
	}
	std::unordered_map<Binding, bool>& getKeyStrokes()
	{
		return keyStrokes;
	}
	GameObject* getPointerToPlayer() const
	{
		return player;
	}
	GameObject* getPointerToActiveEnemy() const
	{
		return activeEnemy;
	}
	int getScore()
	{
		return score;
	}
};

#endif