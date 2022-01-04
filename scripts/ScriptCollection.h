#include "Script.h"
#include <map>

#ifndef SCRIPT_COLLECTION_H
#define SCRIPT_COLLECTION_H

class ScriptCollection
{
private:
	std::vector<Script> scripts;
	std::string collectionName;
public:
	ScriptCollection(std::string collectionName)
	{
		this->collectionName = collectionName;
		std::string name = "Player", spriteName = "player";
		int id = 0, health = 5, damage = 1, score = 0;
		float pos_x = 100.0f, pos_y = 100.0f;
		float rotation = 0.0f;
		float size_x = 40.0f, size_y = 40.0f;
		float vel_x = 0.0f, vel_y = 0.0f;
		float accel_x = 0.0f, accel_y = 0.0f;
		float color_r = 0.0f, color_g = 255.0f;
		float color_b = 255.0f, color_a = 1.0f;
		float minFireDelay = 0.10f;
		std::function<void(GameObject*, float)> updateFunction = [](GameObject* obj, float)
		{
			obj->setVelocity(glm::vec2(0, 0));
		};
		std::vector<float> hitboxInfo
		{ 
			20.5759, 0.7355,
			6.9375, 28.0547,
			34.2568, 28.0547,
			-1.0f, -1.0f,
			6.9375, 28.0547,
			20.5759, 28.0547,
			0.607, 40.7335,
			-1.0f, -1.0f,
			34.2568, 28.0547,
			20.5759, 28.0547,
			40.607, 40.7355,
			-1.0f, -1.0f,
		};

		GameObject player = GameObject(name, id, spriteName, health, damage, score,
		pos_x, pos_y, rotation, size_x, size_y, vel_x, vel_y, accel_x, accel_y,
			color_r, color_g, color_b, color_a);
		player.setUpdateFunction(updateFunction);
		player.setMinFireDelay(minFireDelay);
		std::vector<GameObject> objects{ player };
		std::vector<Command> commands{ Command(ScriptCommand::PLACE_OBJECT, 0) };
		scripts.push_back(Script("Create player", 0, objects, commands));
	}
	std::string getCollectionName() const
	{
		return collectionName;
	}
	Script getScript(size_t at) const
	{
		return scripts.at(at);
	}
	void addScript(Script toAdd)
	{
		scripts.push_back(toAdd);
	}
	size_t numScripts()
	{
		return scripts.size();
	}
	const std::vector<Script> getScripts() const
	{
		return scripts;
	}
};

#endif