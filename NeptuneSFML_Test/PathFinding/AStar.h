#ifndef ASTAR_H
#define ASTAR_H

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

#include <NeptuneSFML\Object\ObjectPool.h>

#include <vector>

#include "Map.h"

//#define BENCH
//#define DRAW_DEBUG
class AStar
{
public:
	struct Node
	{
		sf::Vector2i position;
		float g = 0.f;
		float f = 0.f;
		Node * next = nullptr;
		Node * previous = nullptr;
	};

	enum Status { Waiting = -1, Running = 0, Success, Failure };

	void Init(const Map * _map, float _scale);
	void Draw(sf::RenderTarget & _target);

	Status SearchStep(const sf::Vector2i & _start, const sf::Vector2i & _goal);
	Status Search(const sf::Vector2i & _start, const sf::Vector2i & _goal);

private:
	void Reset(const sf::Vector2i & _start, const sf::Vector2i & _goal);
	int FindMinF();
	float DistanceHeuristic(const Node * _node, const sf::Vector2i & _goal) const;
	void ComputePath(Node * _goal);
	void FindNeighbors(std::vector<Node *> & _neighbors, Node * _currentNode);
	void ComputeNeighbors(Node * _currentNode, const sf::Vector2i & _goal);
#ifdef DRAW_DEBUG
	void CreateDebugShapes();
#endif

	static Node ** FindNode(Node ** _first, size_t _size, Node * _value);

	struct Stats
	{
#ifdef BENCH
		sf::Time timeMinF;
		sf::Time timeHeuristic;
		sf::Time timeFind;
		sf::Time timeFreeObject;
		sf::Clock clockBench;
#endif
		sf::Clock clock;
		sf::Time totalTime;
		unsigned int stepCount = 0;
	};
	
	nep::ObjectPool<Node, 65536> m_nodesPool;
#ifdef DRAW_DEBUG
	nep::ObjectPool<sf::RectangleShape, 65536> m_shapesPool;
#endif
	std::vector<Node *> m_openList;
	std::vector<Node *> m_closedList;
	std::vector<Node *> m_currentNodeNeighbors;
#ifdef DRAW_DEBUG
	std::vector<sf::RectangleShape *> m_debugShapes;
#endif
	Stats m_stats;
	float m_scale;
	Node ** m_currentNeighbor;
	Node ** m_find;
	Node * m_start;
	Node * m_goal;
	Node * m_currentNode;
	const Map * m_map;
	Status m_status;
	int m_neighborCount;
	int m_minF;
};

#endif // !ASTAR_H