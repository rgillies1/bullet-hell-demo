#include "Controller.h"
#include "../game/Unit.h"
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
}

void Controller::processInput(GLFWwindow* window)
{
	float moveSpeed = 500.0f;
	float def_bullet_speed = 1000.0f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (!game.isPaused())
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			moveSpeed = 100.0f;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			player.setVelocity(glm::vec2(player.getVelocity().x, -moveSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			player.setVelocity(glm::vec2(player.getVelocity().x, moveSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			player.setVelocity(glm::vec2(-moveSpeed, player.getVelocity().y));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			player.setVelocity(glm::vec2(moveSpeed, player.getVelocity().y));
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::NORTH, glm::vec2(20, 20), glm::vec2(0, -def_bullet_speed), glm::vec4(0, 255, 255, 1), 0.0f);
			player.setRotation(0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::SOUTH, glm::vec2(20, 20), glm::vec2(0, def_bullet_speed), glm::vec4(0, 255, 255, 1), 0.0f);
			player.setRotation(180.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::EAST, glm::vec2(20, 20), glm::vec2(-def_bullet_speed, 0), glm::vec4(0, 255, 255, 1), 0.0f);
			player.setRotation(270.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::WEST, glm::vec2(20, 20), glm::vec2(def_bullet_speed, 0), glm::vec4(0, 255, 255, 1), 0.0f);
			player.setRotation(90.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		game.togglePause();
	}
}

const std::vector<GameObject>& Controller::updateObjects(float timeSinceLastFrame)
{
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
	}
	return game.getObjects();
}

void Controller::clean()
{
	for (int i = 0; i < game.getObjects().size(); i++)
	{
		if (!game.getObjects().at(i).isAlive())
		{
			game.getObjects().erase(game.getObjects().begin() + i);
		}
	}
}

void Controller::processScripts()
{

}
