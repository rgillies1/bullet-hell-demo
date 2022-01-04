#include "ScriptExecutor.h"

void ScriptExecutor::execute(Game& game, Script script)
{
	for (size_t commandIndex = lastCommandExecuted; commandIndex < script.numCommands();)
	{
		Command nextCommand = script.getCommand(commandIndex);
		if (waiting_for)
		{
			if (glfwGetTime() - timeWaitStarted >= waitTime)
			{
				waiting_for = false;
			}
			else return;
		}
		if (waiting_until)
		{
			if (!game.objectWithIDIsAlive(id_to_wait_for))
			{
				waiting_until = false;
			}
			else return;
		}
		switch (nextCommand.command)
		{
		case ScriptCommand::WAIT_FOR:
			waiting_for = true;
			timeWaitStarted = glfwGetTime();
			waitTime = nextCommand.info;
			break;
		case ScriptCommand::WAIT_UNTIL_DEATH:
			waiting_until = true;
			id_to_wait_for = nextCommand.info;
			break;
		case ScriptCommand::PLACE_OBJECT:
		{
			lastObjectRetreived++;
			GameObject& info = script.getObjectInfo(lastObjectRetreived);
			game.addObject(info);
			break;
		}
		case ScriptCommand::PLACE_AND_WAIT_FOR:
		{
			lastObjectRetreived++;
			GameObject& info = script.getObjectInfo(lastObjectRetreived);
			game.addObject(info);
			waiting_for = true;
			waitTime = nextCommand.info;
			break;
		}

		case ScriptCommand::PLACE_AND_WAIT_UNTIL_DEATH:
		{
			lastObjectRetreived++;
			GameObject& info = script.getObjectInfo(lastObjectRetreived);
			game.addObject(info);
			waiting_until = true;
			id_to_wait_for = info.getID();
			break;
		}
		case ScriptCommand::KILL:
			game.killObjectById(nextCommand.info);
			break;
		}
		commandIndex++;
		lastCommandExecuted = commandIndex;
		if (commandIndex == script.numCommands())
		{
			lastCommandExecuted = -2;
		}
	}
}
