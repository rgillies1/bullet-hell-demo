#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <unordered_map>
#include "../game/GameObject.h"
#include "../graphics/ResourceManager.h"
#include <glm/gtx/hash.hpp>
#include "../game/BulletType.h"
#include "../scripts/Script.h"

class Game
{
private:
	float playableWidth, playableHeight;
	bool paused;
	ResourceManager& manager;

	std::vector<GameObject> objects;
	std::vector<GameObject> temp;

	int lifetimeBulletsFired = 0;
	const int bulletIDBase = 90000;

	void adjustPosition(GameObject& obj);
	GameObject& fireBullet(GameObject& from, Sprite texture, glm::vec2 position,
		glm::vec2 size, glm::vec2 velocity, glm::vec4 color, float rotation,
		std::function<void(GameObject*, float)> bulletfunc = [](GameObject*, float) {});
	void processBullets(GameObject& obj);
public:
	Game(float playableWidth, float playableHeight, ResourceManager& manager);
	~Game();
	std::vector<GameObject>& getObjects();
	void init();
	void updateObjects(float timeSinceLastUpdate);
	void addObject(GameObject& toAdd);
	void addObject(ObjectInfo& info);
	void fireNormalBullet(GameObject& from, BulletType type, Direction direction, glm::vec2 size,
		glm::vec2 velocity, glm::vec4 color, float rotation);
	void fireNormalBullet(GameObject& from, BulletInfo info, std::function<void(GameObject*, float)> bulletFunction);
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
			return objects[id].isAlive();
		}
		catch (std::exception)
		{
			return false;
		}
	}
	void killObjectById(int id)
	{
		objects[id].kill();
	}
};

#endif