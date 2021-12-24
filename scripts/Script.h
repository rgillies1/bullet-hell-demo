#include <queue>
#include "../game/GameObject.h"
#ifndef SCRIPT_H
#define SCRIPT_H

enum class ScriptCommand
{
	WAIT_FOR, WAIT_UNTIL_DEATH, PLACE_OBJECT, PLACE_AND_WAIT_FOR, PLACE_AND_WAIT_UNTIL_DEATH, KILL
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