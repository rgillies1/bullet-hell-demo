#include "ScriptCollection.h"
#ifndef SCRIPT_INITIALIZER_H
#define SCRIPT_INITIALIZER_H

class ScriptInitializer
{
public:
	static ScriptCollection createGameScripts();
	static ScriptCollection createTestScripts();
};

#endif