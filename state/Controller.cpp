#include "Controller.h"
#include <iostream>
#include <algorithm>
int num = 0;
Controller::Controller(Game& game, InputType inputType, ScriptCollection& scripts, ScriptExecutor& executor, Engine& engine, int width, int height) 
	: game(game), scripts(scripts), executor(executor), engine(engine)
{
	this->inputType = inputType;
	this->width = width;
	this->height = height;
}

void Controller::init()
{
	game.init();

	keyBinds.emplace(Binding::PLAYER_MOVE_UP, GLFW_KEY_W);
	keyBinds.emplace(Binding::PLAYER_MOVE_DOWN, GLFW_KEY_S);
	keyBinds.emplace(Binding::PLAYER_MOVE_LEFT, GLFW_KEY_A);
	keyBinds.emplace(Binding::PLAYER_MOVE_RIGHT, GLFW_KEY_D);
	keyBinds.emplace(Binding::PLAYER_FIRE_UP, GLFW_KEY_I);
	keyBinds.emplace(Binding::PLAYER_FIRE_DOWN, GLFW_KEY_K);
	keyBinds.emplace(Binding::PLAYER_FIRE_LEFT, GLFW_KEY_J);
	keyBinds.emplace(Binding::PLAYER_FIRE_RIGHT, GLFW_KEY_L);
	keyBinds.emplace(Binding::PLAYER_SLOW, GLFW_KEY_LEFT_SHIFT);
	keyBinds.emplace(Binding::PAUSE, GLFW_KEY_SPACE);
	keyBinds.emplace(Binding::QUIT, GLFW_KEY_ESCAPE);
}

void Controller::processInput(GLFWwindow* window)
{
	float moveSpeed = 500.0f;
	float def_bullet_speed = 1000.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (!game.isPaused())
	{
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_SLOW]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_SLOW] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_SLOW] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_MOVE_UP]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_MOVE_UP] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_MOVE_UP] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_MOVE_DOWN]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_MOVE_DOWN] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_MOVE_DOWN] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_MOVE_LEFT]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_MOVE_LEFT] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_MOVE_LEFT] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_MOVE_RIGHT]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_MOVE_RIGHT] = true;
		else
		game.getKeyStrokes()[Binding::PLAYER_MOVE_RIGHT] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_FIRE_UP]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_FIRE_UP] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_FIRE_UP] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_FIRE_DOWN]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_FIRE_DOWN] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_FIRE_DOWN] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_FIRE_LEFT]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_FIRE_LEFT] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_FIRE_LEFT] = false;
		if (glfwGetKey(window, keyBinds[Binding::PLAYER_FIRE_RIGHT]) == GLFW_PRESS)
			game.getKeyStrokes()[Binding::PLAYER_FIRE_RIGHT] = true;
		else
			game.getKeyStrokes()[Binding::PLAYER_FIRE_RIGHT] = false;
	}
	if (glfwGetKey(window, keyBinds[Binding::PAUSE]) == GLFW_PRESS)
	{
		game.togglePause();
		isPaused = isPaused ? false : true;
	}
}

const std::unordered_map<int, GameObject>& Controller::updateObjects(float timeSinceLastFrame)
{
	if (game.ended()) isEnded = true;
	game.updateObjects(timeSinceLastFrame);

	if (!game.isPaused())
	{
		engine.doCollisions(game);
		if (executor.getLastExecutedCommand() == -2)
		{
			executor.reset();
			lastExecutedScript++;
		}
		if(lastExecutedScript < scripts.numScripts())
			executor.execute(game, scripts.getScript(lastExecutedScript));

		game.clean();
	}
	return game.getObjects();
}

void Controller::processScripts()
{

}

bool Controller::ended()
{
	return isEnded;
}

bool Controller::paused()
{
	return isPaused;
}

int Controller::playerHealth()
{
	return game.getPointerToPlayer()->getHealth();
}

int Controller::activeEnemyHealth()
{
	if (game.getPointerToActiveEnemy() != nullptr)
		return game.getPointerToActiveEnemy()->getHealth();
	else return -25;
}

int Controller::score()
{
	return game.getScore();
}