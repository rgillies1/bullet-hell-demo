#ifndef GAME_POLYGON_H
#define GAME_POLYGON_H
#include <vector>
#include <iostream>
#include "Shape.h"

class Polygon :
	public Shape
{
private:
	std::vector<glm::vec2> vertices;
	CollisionType  collide(Polygon* s) override { return CollisionType::POLY_POLY; };
	CollisionType  collide(CompositePolygon* s) override { return CollisionType::POLY_COMP; };
	CollisionType  collide(Circle* s) override { return CollisionType::POLY_CIRCLE; };
public:
	Polygon(std::vector<glm::vec2> vertices) 
	{ 
		this->vertices = vertices; 
		this->data = VertexData(vertices);
	};
	void setVertices(std::vector<glm::vec2> vertices) { this->vertices = vertices; };
	std::vector<glm::vec2> getVertices() { return vertices; };
	void translate(float x, float y) override;
	void rotate(float angle) override;
	void scale(float factor) override;
	CollisionType getCollisionType(Shape* s) override { return s->collide(this); };
};

#endif // !GAME_POLYGON_H
