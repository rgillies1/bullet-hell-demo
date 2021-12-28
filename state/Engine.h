#include <vector>
#include <unordered_set>

#include "../game/GameObject.h"
#include "../game/Shapes/Shape.h"
#include "../game/Shapes/Polygon.h"
#include "../game/Shapes/CompositePolygon.h"
#include "../game/Shapes/Circle.h"
#include "Game.h"

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
struct CollisionPair
{
	int id1, id2;
	CollisionPair(int id1, int id2) 
	{
		this->id1 = id1;
		this->id2 = id2;
	}
	bool operator==(const CollisionPair& rhs)
	{
		return (this->id1 == rhs.id1 && this->id2 == rhs.id2) || (this->id2 == rhs.id1 && this->id1 == rhs.id2);
	}
	friend std::ostream& operator<<(std::ostream& os, const CollisionPair& pair)
	{
		return (os << "(" << pair.id1 << ", " << pair.id2 << ")");
	}
};

struct CollisionPairHasher
{
	size_t operator()(const CollisionPair& rhs) const
	{
		return rhs.id1 ^ rhs.id2;
	}
};

struct CollisionPairComparator
{
	bool operator()(const CollisionPair& lhs, const CollisionPair& rhs) const
	{
		return (lhs.id1 == rhs.id1 && lhs.id2 == rhs.id2) || (lhs.id2 == rhs.id1 && lhs.id1 == rhs.id2);
	}
};

class Engine
{
private:
	bool has_seperating_axis(GameObject* a, GameObject* b);
	bool has_seperating_axis(std::unordered_map<int, GameObject>& objects, CollisionPair& pair);
	bool SAT_poly_poly(Shape* poly1, Shape* poly2);
	bool SAT_poly_circle(Shape* poly, Shape* circ);
	bool SAT_poly_comp(Shape* poly, Shape* comp);
	bool SAT_comp_comp(Shape* comp1, Shape* comp2);
	bool SAT_comp_circle(Shape* comp, Shape* circ);
	bool SAT_circle_circle(Shape* circ1, Shape* circ2);
	void sweep_and_prune(std::unordered_map<int, GameObject>& objects,
		std::unordered_set<CollisionPair, CollisionPairHasher, CollisionPairComparator>& collisionPairs,
		std::function<bool(std::pair<int, GameObject>&, std::pair<int, GameObject>&)> sort_function,
		std::function<bool(GameObject&, GameObject&)> prune_function, int bulletBaseID);
public:
	void doCollisions(Game& game);
};

#endif // !GAME_ENGINE_H

