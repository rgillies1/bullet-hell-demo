#include "Polygon.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Polygon::translate(float x, float y)
{
	for (int i = 0; i < vertices.size(); i++)
		vertices[i] += glm::vec2(x, y);
	this->data = VertexData(this->vertices);
}

void Polygon::rotate(float angle)
{
	float cosine = glm::cos(angle);
	float sine = glm::sin(angle);
	for (int i = 0; i < vertices.size(); i++)
	{
		glm::vec2 v = vertices[i];
		float rotX = v.x * cosine - v.y * sine;
		float rotY = v.x * sine + v.y * cosine;
		vertices[i] = glm::vec2(rotX, rotY);
	}
}

void Polygon::scale(float factor)
{
	for (int i = 0; i < vertices.size(); i++)
		vertices[i] *= factor;
}