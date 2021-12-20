#ifndef GAME_SHAPE_H
#define GAME_SHAPE_H
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
class Polygon;
class CompositePolygon;
class Circle;

enum class CollisionType
{
	POLY_POLY,
	POLY_CIRCLE,
	POLY_COMP,
	COMP_POLY,
	COMP_CIRCLE,
	COMP_COMP,
	CIRCLE_POLY,
	CIRCLE_CIRCLE,
	CIRCLE_COMP
};

enum class ShapeType
{
	POLYGON, COMPOSITE_POLYGON, CIRCLE
};

class invalid_shape_data_access_exception : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Attempted to access a non-valid data type";
	}
};

class VertexData
{
private:
	ShapeType type;
	float x;
	float y;
	float radius;
	std::vector<glm::vec2> vertices;
	std::vector<std::vector<glm::vec2>> shapes;
public:
	VertexData() {};
	VertexData(float x, float y, float radius)
	{
		type = ShapeType::CIRCLE;
		this->x = x;
		this->y = y;
		this->radius = radius;
	};
	VertexData(std::vector<glm::vec2> vertices)
	{
		type = ShapeType::POLYGON;
		this->vertices = vertices;
	};
	VertexData(std::vector<std::vector<glm::vec2>> shapes)
	{
		type = ShapeType::COMPOSITE_POLYGON;
		this->shapes = shapes;
	};
	float getCircleX() 
	{ 
		if (type != ShapeType::CIRCLE) throw invalid_shape_data_access_exception();
		return x;
	};
	float getCircleY()
	{
		if (type != ShapeType::CIRCLE) throw invalid_shape_data_access_exception();
		return y;
	};
	float getCircleRadius()
	{
		if (type != ShapeType::CIRCLE) throw invalid_shape_data_access_exception();
		return radius;
	};
	std::vector<glm::vec2> getPolygonVertices()
	{
		if (type != ShapeType::POLYGON) throw invalid_shape_data_access_exception();
		return vertices;
	}	
	std::vector<std::vector<glm::vec2>> getCompPolygonVertices()
	{
		if (type != ShapeType::COMPOSITE_POLYGON) throw invalid_shape_data_access_exception();
		return shapes;
	}
};

class Shape
{
protected:
	VertexData data;
public:
	Shape() {};
	VertexData& getVertexData() { return data; };
	virtual void translate(float, float) = 0;
	virtual void rotate(float) = 0;
	virtual void scale(float) = 0;
	virtual CollisionType getCollisionType(Shape*) = 0;
	virtual CollisionType collide(Circle*) = 0;
	virtual CollisionType collide(Polygon*) = 0;
	virtual CollisionType collide(CompositePolygon*) = 0;
};
#endif // !GAME_SHAPE_H
