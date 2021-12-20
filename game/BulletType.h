#ifndef BULLET_TYPE_H
#define BULLET_TYPE_H
enum class BulletType
{
	CIRCULAR, RECTANGULAR, TRIANGULAR,
};

enum class Direction
{
	NORTH, SOUTH, EAST, WEST
};
struct BulletInfo
{
	BulletType type;
	Direction direction;
	glm::vec2 size, velocity;
	glm::vec4 color;
	float rotation;

	BulletInfo(BulletType type, Direction direction, glm::vec2 size,
		glm::vec2 velocity, glm::vec4 color, float rotation)
	{
		this->type = type;
		this->direction = direction;
		this->size = size;
		this->velocity = velocity;
		this->color = color;
		this->rotation = rotation;
	}
};
#endif