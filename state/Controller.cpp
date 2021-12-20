#include "Controller.h"
#include "../game/Unit.h"
#include <iostream>
#include <algorithm>
int num = 0;
Controller::Controller(Game& game, InputType inputType, ResourceManager& manager, Engine& engine, int width, int height) : game(game), manager(manager), engine(engine)
{
	this->inputType = inputType;
	this->width = width;
	this->height = height;
}

void Controller::init()
{
	game.init();

	manager.generateSprite("diamond", "./graphics/textures/diamond.png", true);

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
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::NORTH, glm::vec2(50, 50), glm::vec2(0, -def_bullet_speed), glm::vec4(1, 1, 1, 1), 0.0f);
			player.setRotation(0.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::SOUTH, glm::vec2(50, 50), glm::vec2(0, def_bullet_speed), glm::vec4(1, 1, 1, 1), 0.0f);
			player.setRotation(180.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::EAST, glm::vec2(50, 50), glm::vec2(-def_bullet_speed, 0), glm::vec4(1, 1, 1, 1), 0.0f);
			player.setRotation(270.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			GameObject& player = game.getObjects().at(0);
			game.fireNormalBullet(player, BulletType::CIRCULAR, Direction::WEST, glm::vec2(50, 50), glm::vec2(def_bullet_speed, 0), glm::vec4(1, 1, 1, 1), 0.0f);
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
	/*for (int i = 0; i < game.getObjects().size(); i++)
	{
		GameObject& obj = game.getObjects().at(i);
		obj.update(timeSinceLastFrame);

		float objX = obj.getPosition().x;
		float objY = obj.getPosition().y;
		if (objX + obj.getSize().x > width || objX < 0.0f 
			|| objY + obj.getSize().y > height || objY < 0.0f)
		{
			if(obj.getName().substr(0, 6) == "bullet")
				obj.kill();
			else
			{
				float newX = objX, newY = objY;

				if (objX + obj.getSize().x > width)
					newX = width - obj.getSize().x;
				if (objX < 0.0f) newX = 0.0f;
				if (objY + obj.getSize().y > height)
					newY = height - obj.getSize().y;
				if (objY < 0.0f) newY = 0.0f;

				obj.setPosition(glm::vec2(newX, newY));
			}
		}
	}*/
	game.updateObjects(timeSinceLastFrame);

	if(!game.isPaused()) engine.doCollisions(game);
	return game.getObjects();
}

void Controller::clean()
{
	for (int i = 0; i < game.getObjects().size(); i++)
	{
		if (!game.getObjects().at(i).isAlive())
		{
			//std::cout << "Killing " << game.getObjects().at(i).getName() << std::endl;
			game.getObjects().erase(game.getObjects().begin() + i);
		}

	}
}

void Controller::makeBullet(GameObject& origin, BulletType type, Sprite sprite, float bulletWidth, float bulletHeight, float posX, float posY)
{
	//const float def_move_speed = 10;
	//float time = glfwGetTime() - origin.getTimeOfLastFire();
	//if (abs(time) < origin.getMinFireDelay()) return;
	//else origin.setTimeOfLastFire(glfwGetTime());
	//glm::vec2 vel = glm::vec2();
	//float rotate = 0.0f;
	//switch (type)
	//{
	//case BulletType::STRAIGHT_DOWN:
	//	vel = glm::vec2(0, def_move_speed);
	//	posX = posX - (bulletWidth / 2);
	//	break;
	//case BulletType::STRAIGHT_LEFT:
	//	vel = glm::vec2(-def_move_speed, 0);
	//	posY = posY - (bulletWidth / 2);
	//	posX = posX - bulletWidth;
	//	rotate = 90.0f;
	//	break;
	//case BulletType::STRAIGHT_UP:
	//	vel = glm::vec2(0, -def_move_speed);
	//	posY = posY - bulletHeight;
	//	posX = posX - (bulletWidth / 2);
	//	break;
	//case BulletType::STRAIGHT_RIGHT:
	//	vel = glm::vec2(def_move_speed, 0);
	//	posY = posY - (bulletWidth / 2);
	//	rotate = 90.0f;
	//	break;
	//}
	//GameObject bullet("bullet"+origin.getName(), ++numBullets + 100000000, sprite, glm::vec2(posX, posY), rotate, glm::vec2(bulletWidth, bulletHeight),
	//	vel , glm::vec4(1.0, 0.0, 0.0, 1.0));
	//bullet.setHitboxCircle(bullet.getPosition().x, bullet.getPosition().y, bullet.getSize().x / 2);
	//bullet.setOriginID(0);
	//game.addObject(bullet);
}
