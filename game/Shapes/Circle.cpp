#include "Circle.h"

#include <iostream>

void Circle::updateVertexData()
{
	this->data = VertexData(this->x, this->y, this->radius);
}

void Circle::translate(float x, float y)
{
	this->x += x;
	this->y += y;
	updateVertexData();
}

void Circle::rotate(float angle)
{
	return;
}

void Circle::scale(float factor)
{
	this->radius *= factor;
}