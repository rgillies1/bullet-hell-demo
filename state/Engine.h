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
	GameObject& obj1;
	GameObject& obj2;
	CollisionPair(GameObject& obj1, GameObject& obj2) : obj1(obj1), obj2(obj2) {}
	bool operator==(const CollisionPair& rhs)
	{
		return (this->obj1 == rhs.obj1 && this->obj2 == rhs.obj2) || (this->obj2 == rhs.obj1 && this->obj1 == rhs.obj2);
	}
	friend std::ostream& operator<<(std::ostream& os, const CollisionPair& pair)
	{
		return (os << "(" << pair.obj1.getID() << ", " << pair.obj2.getID() << ")");
	}
};

struct CollisionPairHasher
{
	size_t operator()(const CollisionPair& rhs) const
	{
		return rhs.obj1.getID() ^ rhs.obj2.getID();
	}
};

struct CollisionPairComparator
{
	bool operator()(const CollisionPair& lhs, const CollisionPair& rhs) const
	{
		return (lhs.obj1 == rhs.obj1 && lhs.obj2 == rhs.obj2) || (lhs.obj2 == rhs.obj1 && lhs.obj1 == rhs.obj2);
	}
};

class Engine
{
private:
	bool has_seperating_axis(GameObject* a, GameObject* b);
	bool has_seperating_axis(CollisionPair& pair);
	bool SAT_poly_poly(Shape* poly1, Shape* poly2);
	bool SAT_poly_circle(Shape* poly, Shape* circ);
	bool SAT_poly_comp(Shape* poly, Shape* comp);
	bool SAT_comp_comp(Shape* comp1, Shape* comp2);
	bool SAT_comp_circle(Shape* comp, Shape* circ);
	bool SAT_circle_circle(Shape* circ1, Shape* circ2);
	void sweep_and_prune(std::vector<GameObject>& objects,
		std::unordered_set<CollisionPair, CollisionPairHasher, CollisionPairComparator>& collisionPairs,
		std::function<bool(GameObject&, GameObject&)> sort_function, 
		std::function<bool(GameObject&, GameObject&)> prune_function, int bulletBaseID);
public:
	void doCollisions(Game& game);
};

#endif // !GAME_ENGINE_H

