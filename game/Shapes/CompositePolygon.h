#ifndef GAME_COMPOSITE_POLYGON_H
#define GAME_COMPOSITE_POLYGON_H
#include "Shape.h"
#include "Polygon.h"
#include <vector>
#include <iostream>
class CompositePolygon :
	public Shape
{
private:
	std::vector<Polygon> polygons;
	void updateVertexData();
	CollisionType collide(Polygon* s) { return CollisionType::COMP_POLY; };
	CollisionType collide(CompositePolygon* s) { return CollisionType::COMP_COMP; };
	CollisionType collide(Circle* s) { return CollisionType::COMP_CIRCLE; };
public:
	CompositePolygon(std::vector<Polygon> polygons) 
	{ 
		this->polygons = polygons; 
		updateVertexData();
	};
	void addPolygon(Polygon toAdd) { polygons.push_back(toAdd); };
	std::vector<Polygon> getPolygons() { return polygons; };
	void translate(float x, float y) override;
	void rotate(float angle) override;
	void scale(float factor) override;
	CollisionType getCollisionType(Shape* s) override { return s->collide(this); };
};

#endif // !GAME_COMPOSITE_POLYGON_H
