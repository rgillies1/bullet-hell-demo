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
		int id = 0;
		float pos_x = 100.0f, pos_y = 100.0f;
		float rotation = 0.0f;
		float size_x = 100.0f, size_y = 100.0f;
		float vel_x = 0.0f, vel_y = 0.0f;
		float accel_x = 0.0f, accel_y = 0.0f;
		float color_r = 0.0f, color_g = 255.0f;
		float color_b = 255.0f, color_a = 1.0f;
		float minFireDelay = 0.10f;
		std::function<void(GameObject*, float)> updateFunction = [](GameObject* obj, float)
		{
			obj->setVelocity(glm::vec2(0, 0));
		};
		std::vector<float> hitboxInfo{ size_x / 2.0f };

		ObjectInfo player(name, id, spriteName,
			pos_x, pos_y, rotation, size_x, size_y,
			vel_x, vel_y, accel_x, accel_y, color_r,
			color_g, color_b, color_a, minFireDelay,
			updateFunction, HitboxShape::CIRCLE,
			hitboxInfo);

		std::vector<ObjectInfo> objects{ player };
		std::vector<Command> commands{ Command(ScriptCommand::PLACE_OBJECT, 0) };
		scripts.push_back(Script("Create player", 0, 0, 0, objects, commands));
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