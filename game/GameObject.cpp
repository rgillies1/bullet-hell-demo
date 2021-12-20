#include "GameObject.h"
#include <iostream>

void GameObject::buildDefaultHitbox()
{
	std::vector<glm::vec2> vert;
	vert.push_back(glm::vec2(this->getPosition()));
	vert.push_back(glm::vec2(this->getPosition().x + this->getSize().x, this->getPosition().y));
	vert.push_back(glm::vec2(this->getPosition() + this->getSize()));
	vert.push_back(glm::vec2(this->getPosition().x, this->getPosition().y + this->getSize().y));
	Polygon box = Polygon(vert);
	hitbox = std::make_shared<Polygon>(box);
}

GameObject::GameObject(std::string name, int id, Sprite sprite, glm::vec2 position, float rotation, glm::vec2 size, glm::vec2 velocity, glm::vec4 color)
	: sprite(sprite)
{
	this->name = name;
	this->id = id;
	this->originId = id;
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->velocity = velocity;
	this->color = color;
	this->alive = true;
	this->timeOfLastFire = 0.0f;
	this->minFireDelay = 0.0f;
	updateFunction = [](GameObject* obj, float timeSinceLastFrame) {};
	buildDefaultHitbox();
}

GameObject::GameObject(std::string name, int id, Sprite sprite, float posX, float posY, float rotation, float sizeX, float sizeY,
	float velocityX, float velocityY, float red, float green, float blue, float alpha)
	: sprite(sprite)
{
	this->name = name;
	this->id = id;
	this->originId = id;
	this->rotation = rotation;
	position = glm::vec2(posX, posY);
	size = glm::vec2(sizeX, sizeY);
	velocity = glm::vec2(velocityX, velocityY);
	color = glm::vec4(red, green, blue, alpha);
	this->alive = true;
	this->timeOfLastFire = 0.0f;
	this->minFireDelay = 0.0f;
	updateFunction = [](GameObject* obj, float timeSinceLastFrame) {};
	buildDefaultHitbox();
}

void GameObject::kill()
{
	alive = false;
}

bool GameObject::isAlive() const
{
	return alive;
}

std::string GameObject::getName() const
{
	return name;
}

int GameObject::getID() const
{
	return id;
}

void GameObject::setOriginID(int originId)
{
	this->originId = originId;
}

int GameObject::getOriginID() const
{
	return originId;
}

Sprite GameObject::getSprite() const
{
	return sprite;
}

void GameObject::setSprite(Sprite sprite)
{
	this->sprite = sprite;
}

glm::vec2 GameObject::getPosition() const
{
	return position;
}

void GameObject::setPosition(glm::vec2 position)
{
	glm::vec2 old(this->getPosition());
	float deltaX = position.x - this->position.x;
	float deltaY = position.y - this->position.y;
	this->position = position;
	old = this->position - old;
	hitbox->translate(old.x, old.y);
}

void GameObject::setPosition(float posX, float posY)
{
	glm::vec2 old(this->getPosition());
	position = glm::vec2(posX, posY);
	old = this->position - old;
	hitbox->translate(old.x, old.y);
}

glm::vec2 GameObject::getSize() const
{
	return size;
}

void GameObject::setSize(glm::vec2 size)
{
	this->size = size;
}

void GameObject::setSize(float sizeX, float sizeY)
{
	size = glm::vec2(sizeX, sizeY);
}

glm::vec2 GameObject::getVelocity() const
{
	return velocity;
}

void GameObject::setVelocity(glm::vec2 velocity)
{
	this->velocity = velocity;
}

void GameObject::setVelocity(float velocityX, float velocityY)
{
	velocity = glm::vec2(velocityX, velocityY);
}

glm::vec4 GameObject::getColor() const
{
	return color;
}

void GameObject::setColor(glm::vec4 color)
{
	this->color = color;
}

void GameObject::setColor(float red, float green, float blue, float alpha)
{
	color = glm::vec4(red, green, blue, alpha);
}

void GameObject::setRotation(float rotation)
{
	this->rotation = rotation;
}

float GameObject::getRotation() const
{
	return rotation;
}

void GameObject::update(float timeSinceLastFrame)
{
	glm::vec2 pos = this->getPosition();
	glm::vec2 vel = this->getVelocity();
	this->setPosition(glm::vec2(pos.x + vel.x * timeSinceLastFrame, pos.y + vel.y * timeSinceLastFrame));
	updateFunction(this, timeSinceLastFrame);
}

void GameObject::setUpdateFunction(std::function<void(GameObject*, float)> updateFunction)
{
	this->updateFunction = updateFunction;
}

float GameObject::getTimeOfLastFire() const
{
	return timeOfLastFire;
}

void GameObject::setTimeOfLastFire(float timeOfLastFire)
{
	this->timeOfLastFire = timeOfLastFire;
}

float GameObject::getMinFireDelay() const
{
	return minFireDelay;
}

void GameObject::setMinFireDelay(float minFireDelay)
{
	this->minFireDelay = minFireDelay;
}

std::shared_ptr<Shape>& GameObject::getHitbox()
{
	return hitbox;
}

std::queue<std::pair<BulletInfo, std::function<void(GameObject*, float)>>>& GameObject::getBulletsToFire()
{
	return toFire;
}

void GameObject::setHitboxPolygon(std::vector<glm::vec2> vertices)
{
	this->hitbox.reset(new Polygon(vertices));
}

void GameObject::setHitboxCompositePolygon(std::vector<Polygon> shapes)
{
	this->hitbox.reset(new CompositePolygon(shapes));
}

void GameObject::setHitboxCircle(float x, float y, float radius)
{
	this->hitbox.reset(new Circle(x + radius, y + radius, radius));
}

