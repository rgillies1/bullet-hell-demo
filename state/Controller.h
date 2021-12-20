#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Game.h"
#include "../game/Shapes/Polygon.h"
#include "../game/Shapes/Circle.h"
#include "../game/Shapes/CompositePolygon.h"
#include "Engine.h"
enum class InputType
{
	INPUT_KEYBOARD, INPUT_GAMEPAD
};
//enum class BulletType
//{
//	STRAIGHT_UP,
//	STRAIGHT_DOWN,
//	STRAIGHT_RIGHT,
//	STRAIGHT_LEFT
//};
class Controller
{
private:
	Game& game;
	InputType inputType;
	ResourceManager& manager;
	Engine& engine;
	int width, height;
	int numBullets = 0;
public:
	Controller(Game& game, InputType inputType, ResourceManager& manager, Engine& engine, int width, int height);
	void init();
	void processInput(GLFWwindow* window);
	const std::vector<GameObject>& updateObjects(float timeSinceLastFrame);
	void makeBullet(GameObject& origin, BulletType type, Sprite sprite, float bulletWidth, float bulletHeight, float posX, float posY);
	void clean();
};

#endif