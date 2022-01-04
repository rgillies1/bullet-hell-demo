#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>

#include "Game.h"
#include "../game/Shapes/Polygon.h"
#include "../game/Shapes/Circle.h"
#include "../game/Shapes/CompositePolygon.h"
#include "Engine.h"
#include "../scripts/ScriptCollection.h"
#include "../scripts/ScriptExecutor.h"
#include "Bindings.h"

enum class InputType
{
	INPUT_KEYBOARD, INPUT_GAMEPAD
};
class Controller
{
private:
	Game& game;
	InputType inputType;
	Engine& engine;
	ScriptCollection& scripts;
	ScriptExecutor& executor;
	std::unordered_map<Binding, int> keyBinds;
	int width, height;
	int numBullets = 0, lastExecutedScript = 0;
	bool isEnded, isPaused;
public:
	Controller(Game& game, InputType inputType, ScriptCollection& scripts, ScriptExecutor& executor, Engine& engine, int width, int height);
	void init();
	void processInput(GLFWwindow* window);
	const std::unordered_map<int, GameObject>& updateObjects(float timeSinceLastFrame);
	void processScripts();
	bool ended();
	bool paused();
	int playerHealth();
	int activeEnemyHealth();
	int score();
};

#endif