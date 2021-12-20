#ifndef GAME_CIRCLE_H
#define GAME_CIRCLE_H
#include "Shape.h"
#include <iostream>
class Circle :
	public Shape
{
private:
	float x, y, radius;
	void updateVertexData();
	CollisionType collide(Polygon* s) { return CollisionType::CIRCLE_POLY; };
	CollisionType collide(CompositePolygon* s) { return CollisionType::CIRCLE_COMP; };
	CollisionType collide(Circle* s) { return CollisionType::CIRCLE_CIRCLE; };
public:
	Circle(float x, float y, float radius)
	{
		this->x = x;
		this->y = y;
		this->radius = radius;
		updateVertexData();
	};
	glm::vec2 getCenter() { return glm::vec2(x, y); };
	float getRadius() { return radius; };
	void translate(float x, float y) override;
	void rotate(float angle) override;
	void scale(float factor) override;
	CollisionType getCollisionType(Shape* s) override { return s->collide(this); };
};

#endif // !GAME_CIRCLE_H
