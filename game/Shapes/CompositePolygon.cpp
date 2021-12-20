#include "CompositePolygon.h"

void CompositePolygon::updateVertexData()
{
	std::vector<std::vector<glm::vec2>> poly;
	for (int i = 0; i < polygons.size(); i++)
	{
		Polygon p = polygons[i];
		std::vector<glm::vec2> vertices = p.getVertices();
		poly.push_back(vertices);
	}
	this->data = VertexData(poly);
}

void CompositePolygon::translate(float x, float y)
{
	for (int i = 0; i < polygons.size(); i++)
		polygons[i].translate(x, y);
	updateVertexData();
}

void CompositePolygon::rotate(float angle)
{
	for (int i = 0; i < polygons.size(); i++)
		polygons[i].rotate(angle);
}

void CompositePolygon::scale(float factor)
{
	for (int i = 0; i < polygons.size(); i++)
		polygons[i].scale(factor);
}
