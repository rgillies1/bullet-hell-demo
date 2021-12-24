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
public:
	Controller(Game& game, InputType inputType, ScriptCollection& scripts, ScriptExecutor& executor, Engine& engine, int width, int height);
	void init();
	void processInput(GLFWwindow* window);
	const std::vector<GameObject>& updateObjects(float timeSinceLastFrame);
	void makeBullet(GameObject& origin, BulletType type, Sprite sprite, float bulletWidth, float bulletHeight, float posX, float posY);
	void processScripts();
	void clean();
};

#endif