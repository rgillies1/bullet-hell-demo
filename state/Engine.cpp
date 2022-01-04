#include <algorithm>
#include <unordered_set>
#include "Engine.h"

bool Engine::has_seperating_axis(GameObject* a, GameObject* b)
{
	Shape* hitbox1 = a->getHitbox().get();
	Shape* hitbox2 = b->getHitbox().get();
	assert(hitbox1 != nullptr && hitbox2 != nullptr);
	CollisionType type = hitbox1->getCollisionType(hitbox2);

	switch (type)
	{
	case CollisionType::POLY_POLY:
		return SAT_poly_poly(hitbox1, hitbox2);
	case CollisionType::POLY_COMP:
		return SAT_poly_comp(hitbox2, hitbox1);
	case CollisionType::POLY_CIRCLE:
		return SAT_poly_circle(hitbox2, hitbox1);
	case CollisionType::COMP_POLY:
		return SAT_poly_comp(hitbox1, hitbox2);
	case CollisionType::COMP_COMP:
		return SAT_comp_comp(hitbox1, hitbox2);
	case CollisionType::COMP_CIRCLE:
		return SAT_comp_circle(hitbox2, hitbox1);
	case CollisionType::CIRCLE_POLY:
		return SAT_poly_circle(hitbox1, hitbox2);
	case CollisionType::CIRCLE_COMP:
		return SAT_comp_circle(hitbox1, hitbox2);
	case CollisionType::CIRCLE_CIRCLE:
		return SAT_circle_circle(hitbox1, hitbox2);
	}
	return false;
}

bool Engine::has_seperating_axis(std::unordered_map<int, GameObject>& objects, CollisionPair& pair)
{
	GameObject& obj1 = objects.at(pair.id1);
	GameObject& obj2 = objects.at(pair.id2);
	return has_seperating_axis(&obj1, &obj2);
}

bool Engine::SAT_poly_poly(Shape* poly1, Shape* poly2)
{
	std::vector<glm::vec2> p1vertices = poly1->getVertexData().getPolygonVertices();
	std::vector<glm::vec2> p2vertices = poly2->getVertexData().getPolygonVertices();
	if (p2vertices.size() == 0 || p1vertices.size() == 0) return true;
	for (int i = 0; i < p1vertices.size(); i++)
	{

		glm::vec2 v1 = p1vertices[i];
		glm::vec2 v2 = p1vertices[(i + 1) % p1vertices.size()];
		glm::vec2 edge = v2 - v1;
		glm::vec2 normal(-edge.y, edge.x);

		float min1 = std::numeric_limits<float>::infinity();
		float max1 = -std::numeric_limits<float>::infinity();
		float min2 = std::numeric_limits<float>::infinity();
		float max2 = -std::numeric_limits<float>::infinity();

		for (glm::vec2 v : p1vertices)
		{
			float projection = glm::dot(normal, v);
			if (projection > max1) max1 = projection;
			if (projection < min1) min1 = projection;
		}

		for (glm::vec2 v : p2vertices)
		{
			float projection = glm::dot(normal, v);
			if (projection > max2) max2 = projection;
			if (projection < min2) min2 = projection;
		}

		if (min1 > max2 || min2 > max1)
			return true;
	}

	for (int i = 0; i < p2vertices.size(); i++)
	{
		glm::vec2 v1 = p2vertices[i];
		glm::vec2 v2 = p2vertices[(i + 1) % p2vertices.size()];
		glm::vec2 edge = v2 - v1;
		glm::vec2 normal(-edge.y, edge.x);

		float min1 = std::numeric_limits<float>::infinity();
		float max1 = -std::numeric_limits<float>::infinity();
		float min2 = std::numeric_limits<float>::infinity();
		float max2 = -std::numeric_limits<float>::infinity();

		for (glm::vec2 v : p1vertices)
		{
			float projection = glm::dot(normal, v);
			if (projection > max1) max1 = projection;
			if (projection < min1) min1 = projection;
		}

		for (glm::vec2 v : p2vertices)
		{
			float projection = glm::dot(normal, v);
			if (projection > max2) max2 = projection;
			if (projection < min2) min2 = projection;
		}

		if (min1 > max2 || min2 > max1)
			return true;
	}

	return false;
}

bool Engine::SAT_poly_circle(Shape* poly, Shape* circ)
{
	const std::vector<glm::vec2> polyVertices = poly->getVertexData().getPolygonVertices();
	const glm::vec2 circCenter = glm::vec2(circ->getVertexData().getCircleX(), circ->getVertexData().getCircleY());
	glm::vec2 closestPoint(-1, -1);
	glm::vec2 shortestDelta(-1, -1);
	float shortestDistanceToPoint = std::numeric_limits<float>::infinity();
	float min1 = std::numeric_limits<float>::infinity();
	float max1 = -std::numeric_limits<float>::infinity();
	float min2 = std::numeric_limits<float>::infinity();
	float max2 = -std::numeric_limits<float>::infinity();

	for (int i = 0; i < polyVertices.size(); i++)
	{
		glm::vec2 v1 = polyVertices[i];
		glm::vec2 v2 = polyVertices[(i + 1) % polyVertices.size()];
		glm::vec2 edge = v2 - v1;
		glm::vec2 normal(-edge.y, edge.x);

		normal = glm::normalize(normal);

		min1 = std::numeric_limits<float>::infinity();
		max1 = -std::numeric_limits<float>::infinity();
		min2 = std::numeric_limits<float>::infinity();
		max2 = -std::numeric_limits<float>::infinity();

		for (glm::vec2 v : polyVertices)
		{
			float projection = glm::dot(normal, v);
			if (projection > max1) max1 = projection;
			if (projection < min1) min1 = projection;
		}

		glm::vec2 circProjection = glm::normalize(normal) * circ->getVertexData().getCircleRadius();
		min2 = glm::dot(circCenter - circProjection, normal);
		max2 = glm::dot(circCenter + circProjection, normal);

		if (max2 < min2)
		{
			float t = max2;
			max2 = min2;
			min2 = t;
		}

		if (min1 > max2 || min2 > max1)
			return true;
	}

	for (glm::vec2 v : polyVertices)
	{
		glm::vec2 delta(v.x - circCenter.x, v.y - circCenter.y);
		float distance = glm::length(delta);

		if (distance < shortestDistanceToPoint)
		{
			shortestDistanceToPoint = distance;
			closestPoint = v;
			shortestDelta = delta;
		}
	}

	min1 = std::numeric_limits<float>::infinity();
	max1 = -std::numeric_limits<float>::infinity();
	min2 = std::numeric_limits<float>::infinity();
	max2 = -std::numeric_limits<float>::infinity();

	glm::vec2 closestAxis = closestPoint - circCenter;
	closestAxis = glm::normalize(closestAxis);

	for (glm::vec2 v : polyVertices)
	{
		float projection = glm::dot(closestAxis, v);
		if (projection > max1) max1 = projection;
		if (projection < min1) min1 = projection;
	}

	glm::vec2 direction = glm::normalize(closestAxis);
	glm::vec2 dirAndRadius = direction * circ->getVertexData().getCircleRadius();

	glm::vec2 p1 = circCenter + dirAndRadius;
	glm::vec2 p2 = circCenter - dirAndRadius;

	min2 = glm::dot(p1, closestAxis);
	max2 = glm::dot(p2, closestAxis);

	if (max2 < min2)
	{
		float t = max2;
		max2 = min2;
		min2 = t;
	}

	if (min1 > max2 || min2 > max1)
		return true;
	return false;
}

bool Engine::SAT_poly_comp(Shape* poly, Shape* comp)
{
	std::vector<std::vector<glm::vec2>> shapes = comp->getVertexData().getCompPolygonVertices();
	int iter = 0;
	for (std::vector<glm::vec2> vec : shapes)
	{
		Polygon p = Polygon(vec);
		if (!SAT_poly_poly(poly, &p))
		{
			std::cout << "SAT returned false at shape " << iter << "." << std::endl;
			return false;
		};
		iter++;
	}
	return true;
}

bool Engine::SAT_comp_comp(Shape* comp1, Shape* comp2)
{
	std::vector<std::vector<glm::vec2>> shapes1 = comp1->getVertexData().getCompPolygonVertices();
	std::vector<std::vector<glm::vec2>> shapes2 = comp2->getVertexData().getCompPolygonVertices();
	for (std::vector<glm::vec2> vec1 : shapes1)
	{
		Polygon p1 = Polygon(vec1);
		for (std::vector<glm::vec2> vec2 : shapes2)
		{
			Polygon p2 = Polygon(vec2);
			if (!SAT_poly_poly(&p1, &p2)) return false;
		}
	}
	return true;
}

bool Engine::SAT_comp_circle(Shape* comp, Shape* circ)
{
	std::vector<std::vector<glm::vec2>> shapes = comp->getVertexData().getCompPolygonVertices();
	for (std::vector<glm::vec2> vec : shapes)
	{
		Polygon p = Polygon(vec);
		if (!SAT_poly_circle(&p, circ)) return false;
	}
	return true;
}

bool Engine::SAT_circle_circle(Shape* circ1, Shape* circ2)
{
	float rad1 = circ1->getVertexData().getCircleRadius();
	float rad2 = circ2->getVertexData().getCircleRadius();
	glm::vec2 center1 = glm::vec2(circ1->getVertexData().getCircleX(), circ1->getVertexData().getCircleY());
	glm::vec2 center2 = glm::vec2(circ2->getVertexData().getCircleX(), circ2->getVertexData().getCircleY());

	if (glm::length(center1 - center2) < rad1 + rad2)
		return false;
	else return true;
}

void Engine::sweep_and_prune(std::unordered_map<int, GameObject>& objects,
	std::unordered_set<CollisionPair, CollisionPairHasher, CollisionPairComparator>& collisionPairs,
	std::function<bool(std::pair<int, GameObject>&, std::pair<int, GameObject>&)> sort_function,
	std::function<bool(GameObject&, GameObject&)> prune_function, int bulletBaseID)
{
	std::list<GameObject*> sweep;
	std::vector<std::pair<int, GameObject>> sortedObjects(objects.begin(), objects.end());
	std::sort(sortedObjects.begin(), sortedObjects.end(), sort_function);

	GameObject& first = sortedObjects.at(0).second;
	sweep.push_back(&first);
	for (int i = 1; i < objects.size(); i++)
	{
		GameObject& next = sortedObjects.at(i).second;
		for (auto it = sweep.begin(); it != sweep.end();)
		{
			if (**it == next)
			{
				it++;
				continue;
			}
			if (prune_function(next, **it))
			{
				it = sweep.erase(it);
			}
			else
			{
				if (next.getOriginID() != (*it)->getOriginID() && !(next.getID() >= bulletBaseID && (*it)->getID() >= bulletBaseID))
				{
					collisionPairs.insert(CollisionPair(next.getID(), (*it)->getID()));
				}
				it++;
			}
		}
		sweep.push_back(&next);
	}
}

bool sweepX(std::pair<int, GameObject>& obj1, std::pair<int, GameObject>& obj2)
{
	return obj1.second.getPosition().x < obj2.second.getPosition().x;
}

bool sweepY(std::pair<int, GameObject>& obj1, std::pair<int, GameObject>& obj2)
{
	return obj1.second.getPosition().y < obj2.second.getPosition().y;
}

bool pruneX(GameObject& obj1, GameObject& obj2)
{
	return obj1.getPosition().x > obj2.getPosition().x + obj2.getSize().x;
}

bool pruneY(GameObject& obj1, GameObject& obj2)
{
	return obj1.getPosition().y > obj2.getPosition().y + obj2.getSize().y;
}

void Engine::doCollisions(Game& game)
{
	if (game.getObjects().size() > 1)
	{
		std::list<GameObject*> sweep;
		std::unordered_set<CollisionPair, CollisionPairHasher, CollisionPairComparator> collisionPairs;

		std::unordered_map<int, GameObject> x_objects = game.getObjects();
		std::unordered_map<int, GameObject> y_objects = game.getObjects();
		int bulletBaseID = game.getBulletIdBase();

		sweep_and_prune(x_objects, collisionPairs, sweepX, pruneX, bulletBaseID);
		sweep_and_prune(y_objects, collisionPairs, sweepY, pruneY, bulletBaseID);

		for (CollisionPair pair : collisionPairs)
		{
			if (!has_seperating_axis(game.getObjects(), pair))
			{
				if (pair.id1 == 0)
				{
					std::cout << "POSITION: " << game.getObjects().at(pair.id1).getPosition().x << ", " << game.getObjects().at(pair.id1).getPosition().y << std::endl;
					for (Polygon p : game.getObjects().at(pair.id1).getHitbox()->getVertexData().getCompPolygonVertices())
					{
						for (glm::vec2 v : p.getVertices())
						{
							std::cout << v.x << ", " << v.y << std::endl;
						}
					}
				}
				if (pair.id2 == 0)
				{
					std::cout << "POSITION:" << game.getObjects().at(pair.id2).getPosition().x << ", " << game.getObjects().at(pair.id2).getPosition().y << std::endl;
					for (Polygon p : game.getObjects().at(pair.id2).getHitbox()->getVertexData().getCompPolygonVertices())
					{
						for (glm::vec2 v : p.getVertices())
						{
							std::cout << v.x << ", " << v.y << std::endl;
						}
					}
				}
				game.getObjects().at(pair.id1).hurt(1);
				game.getObjects().at(pair.id2).hurt(1);
			}
		}

		// Collisions are done
		collisionPairs.clear();
	}
}