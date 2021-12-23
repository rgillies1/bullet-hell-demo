#include "../state/Game.h"
#include "Script.h"
#ifndef SCRIPT_EXECUTOR_H
#define SCRIPT_EXECUTOR_H
class ScriptExecutor
{
private:
	bool waiting_for = false, waiting_until = false;
	float waitTime, timeWaitStarted;
	int id_to_wait_for, lastCommandExecuted = 0, lastObjectRetreived = -1;
public:
	void execute(Game& game, Script script);
	int getLastExecutedCommand()
	{
		return lastCommandExecuted;
	}
	void reset()
	{
		lastCommandExecuted = 0;
		lastObjectRetreived = -1;
	}
};
#endif