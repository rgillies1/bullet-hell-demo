#include <queue>
#include "../game/GameObject.h"
#ifndef SCRIPT_H
#define SCRIPT_H

enum class ScriptCommand
{
	WAIT_FOR, WAIT_UNTIL_DEATH, PLACE_OBJECT, PLACE_AND_WAIT_FOR, PLACE_AND_WAIT_UNTIL_DEATH, KILL
};

enum class HitboxShape
{
	CIRCLE, POLYGON, COMPOSITE_POLYGON
};

struct ObjectInfo
{
	std::string name, spriteName;
	int id;
	float vel_x, vel_y, size_x, size_y, pos_x, pos_y, color_r, color_g, color_b, color_a, rotation, def_minFireDelay;
	std::function<void(GameObject*, float)> updateFunction;
	HitboxShape hitboxShape;
	std::vector<float> hitboxInfo;
	ObjectInfo(std::string name, 
		int id, std::string spriteName, 
		float pos_x, float pos_y, 
		float rotation, float size_x, 
		float size_y, float vel_x,
		float vel_y, float color_r,
		float color_g, float color_b,
		float color_a, float minFireDelay,
		std::function<void(GameObject*, float)> updateFunction,
		HitboxShape hitboxShape,
		std::vector<float> hitboxInfo)
	{
		this->name = name;
		this->id = id;
		this->spriteName = spriteName;
		this->pos_x = pos_x;
		this->pos_y = pos_y;
		this->rotation = rotation;
		this->size_x = size_x;
		this->size_y = size_y;
		this->vel_x = vel_x;
		this->vel_y = vel_y;
		this->color_r = color_r;
		this->color_b = color_b;
		this->color_g = color_g;
		this->color_a = color_a;
		this->def_minFireDelay = minFireDelay;
		this->updateFunction = updateFunction;
		this->hitboxShape = hitboxShape;
		this->hitboxInfo = hitboxInfo;
	}
};


struct Command
{
	ScriptCommand command;
	int info;
	Command(ScriptCommand command, int info)
	{
		this->command = command;
		this->info = info;
	}
};

class Script
{
private:
	std::string scriptName;
	int executionOrderLabel;
	float beginWaitTime, endWaitTime;
	std::vector<ObjectInfo> objects;
	std::vector<Command> commands;
public:
	Script(std::string scriptName, int executionOrderLabel, float beginWaitTime, float endWaitTime,
		std::vector<ObjectInfo> objects, std::vector<Command> commands)
	{
		this->scriptName = scriptName;
		this->executionOrderLabel = executionOrderLabel;
		this->beginWaitTime = beginWaitTime;
		this->endWaitTime = endWaitTime;
		this->objects = objects;
		this->commands = commands;
	};
	std::string getScriptName() const
	{
		return scriptName;
	};
	int getExecOrderLabel() const
	{
		return executionOrderLabel;
	};
	Command getCommand(int index)
	{
		return commands[index];
	};
	ObjectInfo& getObjectInfo(int index)
	{
		return objects[index];
	};
	size_t numCommands() const
	{
		return commands.size();
	}
	size_t numObjects() const
	{
		return objects.size();
	}
	bool operator==(const Script& rhs) const
	{
		return this->executionOrderLabel == rhs.executionOrderLabel;
	}
	bool operator<(const Script& rhs) const
	{
		return this->executionOrderLabel < rhs.executionOrderLabel;
	}
};

#endif