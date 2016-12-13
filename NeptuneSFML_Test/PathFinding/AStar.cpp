#include "AStar.h"

#include <NeptuneSFML\Object\SpriteBatch.h>

#include <algorithm>

#define NORMAL_COST 1.f
#define DIAGONAL_COST 2.f
#define EIGHT_WAY
//#define DIAGONAL_DISTANCE
//#define EUCLIDIAN
#define EUCLIDIAN_SQUARE

void AStar::Init(const Map * _map, float _scale)
{
	m_map = _map;
	m_status = Status::Waiting;
	m_scale = _scale;
}

void AStar::Draw(sf::RenderTarget & _target)
{
#ifdef DRAW_DEBUG
	nep::SpriteBatch::Draw(_target, m_debugShapes);
#endif
}

AStar::Status AStar::Search(const sf::Vector2i & _start, const sf::Vector2i & _goal)
{	
	m_stats.clock.restart();

	if (m_status != Status::Running)
		Reset(_start, _goal);

	while (!m_openList.empty())
	{
		m_stats.stepCount++;

#ifdef BENCH
		m_stats.clockBench.restart();
		m_minF = FindMinF();
		m_currentNode = m_openList[m_minF];
		m_openList[m_minF] = m_openList.back();
		m_openList.back() = m_currentNode;
		m_stats.timeMinF += m_stats.clockBench.getElapsedTime();
#else
		m_minF = FindMinF();
		m_currentNode = m_openList[m_minF];
		m_openList[m_minF] = m_openList.back();
		m_openList.back() = m_currentNode;
#endif

		if (m_currentNode->position == _goal)
		{
			ComputePath(m_currentNode);
			m_stats.totalTime = m_stats.clock.getElapsedTime();
			m_status = Status::Success;

#ifdef BENCH
			printf("Time min f = %lld us\nTime heuristic = %lld us\nTime find = %lld us\nTime free object = %lld us\n\n",
				m_stats.timeMinF.asMicroseconds(), m_stats.timeHeuristic.asMicroseconds(), m_stats.timeFind.asMicroseconds(), m_stats.timeFreeObject.asMicroseconds());
#endif

			printf("Stats :\n\tTotal time = %lld us\n\tStep count = %d\n", m_stats.totalTime.asMicroseconds(), m_stats.stepCount);
#ifdef DRAW_DEBUG
			CreateDebugShapes();
#endif
			return m_status;
		}

		m_openList.pop_back();
		m_closedList.push_back(m_currentNode);

		ComputeNeighbors(m_currentNode, _goal);
	}

	m_stats.totalTime = m_stats.clock.getElapsedTime();
	m_status = Status::Failure;

#ifdef BENCH
	printf("Time min f = %lld us\nTime heuristic = %lld us\nTime find = %lld us\nTime free object = %lld us\n\n",
		m_stats.timeMinF.asMicroseconds(), m_stats.timeHeuristic.asMicroseconds(), m_stats.timeFind.asMicroseconds(), m_stats.timeFreeObject.asMicroseconds());
#endif

	printf("Stats :\n\tTotal time = %lld us\n\tStep count = %d\n", m_stats.totalTime.asMicroseconds(), m_stats.stepCount);
#ifdef DRAW_DEBUG
	CreateDebugShapes();
#endif
	return m_status;
}

AStar::Status AStar::SearchStep(const sf::Vector2i & _start, const sf::Vector2i & _goal)
{
	m_stats.clock.restart();

	if (m_status != Status::Running)
		Reset(_start, _goal);

	if (!m_openList.empty())
	{
		m_stats.stepCount++;

#ifdef BENCH
		m_stats.clockBench.restart();
		m_minF = FindMinF();
		m_currentNode = m_openList[m_minF];
		m_openList[m_minF] = m_openList.back();
		m_openList.back() = m_currentNode;
		m_stats.timeMinF += m_stats.clockBench.getElapsedTime();
#else
		m_minF = FindMinF();
		m_currentNode = m_openList[m_minF];
		m_openList[m_minF] = m_openList.back();
		m_openList.back() = m_currentNode;
#endif

		if (m_currentNode->position == _goal)
		{
			ComputePath(m_currentNode);
			m_stats.totalTime += m_stats.clock.getElapsedTime();
			m_status = Status::Success;

#ifdef BENCH
			printf("Time min f = %lld us\nTime heuristic = %lld us\nTime find = %lld us\nTime free object = %lld us\n\n",
				m_stats.timeMinF.asMicroseconds(), m_stats.timeHeuristic.asMicroseconds(), m_stats.timeFind.asMicroseconds(), m_stats.timeFreeObject.asMicroseconds());
#endif

			printf("Stats :\n\tTotal time = %lld us\n\tStep count = %d\n\n", m_stats.totalTime.asMicroseconds(), m_stats.stepCount);
#ifdef DRAW_DEBUG
			CreateDebugShapes();
#endif
			return m_status;
		}

		m_openList.pop_back();
		m_closedList.push_back(m_currentNode);

		ComputeNeighbors(m_currentNode, _goal);

		m_stats.totalTime += m_stats.clock.getElapsedTime();
#ifdef DRAW_DEBUG
		CreateDebugShapes();
#endif
		return m_status;
	}

	m_stats.totalTime += m_stats.clock.getElapsedTime();
	m_status = Status::Failure;

#ifdef BENCH
	printf("Time min f = %lld us\nTime heuristic = %lld us\nTime find = %lld us\nTime free object = %lld us\n\n",
		m_stats.timeMinF.asMicroseconds(), m_stats.timeHeuristic.asMicroseconds(), m_stats.timeFind.asMicroseconds(), m_stats.timeFreeObject.asMicroseconds());
#endif

	printf("Stats :\n\tTotal time = %lld us\n\tStep count = %d\n", m_stats.totalTime.asMicroseconds(), m_stats.stepCount);
#ifdef DRAW_DEBUG
	CreateDebugShapes();
#endif
	return m_status;
}

void AStar::Reset(const sf::Vector2i & _start, const sf::Vector2i & _goal)
{
	m_status = Status::Running;
	m_nodesPool.FreeAllObjects();
	m_openList.clear();
	m_closedList.clear();
#ifdef DRAW_DEBUG
	m_shapesPool.FreeAllObjects();
	m_debugShapes.clear();
#endif

	m_start = m_nodesPool.GetObject();
	m_start->position = _start;
	m_start->g = 0.f;


#ifdef BENCH
	m_stats.clockBench.restart();
	m_start->f = DistanceHeuristic(m_start, _goal);
	m_stats.timeHeuristic += m_stats.clockBench.getElapsedTime();
#else
	m_start->f = DistanceHeuristic(m_start, _goal);
#endif

	m_start->previous = nullptr;
	m_start->next = nullptr;
	m_openList.push_back(m_start);

	m_goal = nullptr;

	m_stats.stepCount = 0;
#ifdef BENCH
	m_stats.timeFind = sf::Time::Zero;
	m_stats.timeFreeObject = sf::Time::Zero;
	m_stats.timeHeuristic = sf::Time::Zero;
	m_stats.timeMinF = sf::Time::Zero;
#endif
	m_stats.totalTime = sf::Time::Zero;
}

int AStar::FindMinF()
{
	int i = m_openList.size() - 1;
	int minF = 0;
	for (; i >= 0; i--)
	{
		if (m_openList[i]->f < m_openList[minF]->f)
			minF = i;
	}

	return minF;
}

float AStar::DistanceHeuristic(const Node * _node, const sf::Vector2i & _goal) const
{
#ifdef DIAGONAL_DISTANCE
	const int AbsDistX = abs(_node->position.x - _goal.x);
	const int AbsDistY = abs(_node->position.y - _goal.y);

#ifdef EIGHT_WAY
	const int maxDist = std::max(AbsDistX, AbsDistY);
	const int minDist = std::min(AbsDistX, AbsDistY);

	return static_cast<float>(DIAGONAL_COST * minDist + NORMAL_COST * (maxDist - minDist));
#else
	return static_cast<float>(AbsDistX + AbsDistY);
#endif // EIGHT_WAY
#endif // DIAGONAL_DISTANCE

#ifdef EUCLIDIAN
	return std::sqrtf(static_cast<float>((_goal.x - _node->position.x) * (_goal.x - _node->position.x) + (_goal.y - _node->position.y) * (_goal.y - _node->position.y)));
#endif // EUCLIDIAN_SQUARE

#ifdef EUCLIDIAN_SQUARE
	return static_cast<float>((_goal.x - _node->position.x) * (_goal.x - _node->position.x) + (_goal.y - _node->position.y) * (_goal.y - _node->position.y));
#endif // EUCLIDIAN
}

void AStar::ComputePath(Node * _goal)
{
	Node * m_currentNode = nullptr;
	m_goal = _goal;
	m_currentNode = m_goal;

	while (m_currentNode->previous != nullptr)
	{
		m_currentNode->previous->next = m_currentNode;
		m_currentNode = m_currentNode->previous;
	}
}

void AStar::FindNeighbors(std::vector<Node *> & _neighbors, Node * _currentNode)
{
	const sf::Vector2i position = _currentNode->position;
	const float normalG = _currentNode->g + NORMAL_COST;
	const float diagonalG = _currentNode->g + DIAGONAL_COST;
	//const float normalG = NORMAL_COST;
	//const float diagonalG = DIAGONAL_COST;
	Node * newNode = nullptr;

	if (m_map->IsWalkable(position.x, position.y - 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x;
		newNode->position.y = position.y - 1;
		newNode->g = normalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x + 1, position.y))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x + 1;
		newNode->position.y = position.y;
		newNode->g = normalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x, position.y + 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x;
		newNode->position.y = position.y + 1;
		newNode->g = normalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x - 1, position.y))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x - 1;
		newNode->position.y = position.y;
		newNode->g = normalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

#ifdef EIGHT_WAY
	if (m_map->IsWalkable(position.x + 1, position.y - 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x + 1;
		newNode->position.y = position.y - 1;
		newNode->g = diagonalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x + 1, position.y + 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x + 1;
		newNode->position.y = position.y + 1;
		newNode->g = diagonalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x - 1, position.y + 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x - 1;
		newNode->position.y = position.y + 1;
		newNode->g = diagonalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}

	if (m_map->IsWalkable(position.x - 1, position.y - 1))
	{
		newNode = m_nodesPool.GetObject();
		newNode->position.x = position.x - 1;
		newNode->position.y = position.y - 1;
		newNode->g = diagonalG;
		newNode->previous = _currentNode;
		_neighbors.push_back(newNode);
	}
#endif // EIGHT_WAY
}

void AStar::ComputeNeighbors(Node * _currentNode, const sf::Vector2i & _goal)
{
	m_currentNodeNeighbors.clear();
	FindNeighbors(m_currentNodeNeighbors, _currentNode);

	m_neighborCount = m_currentNodeNeighbors.size();
	m_currentNeighbor = m_currentNodeNeighbors.data();

	for (int i = 0; i < m_neighborCount; i++, m_currentNeighbor++)
	{
#ifdef BENCH
		m_stats.clockBench.restart();
		m_find = FindNode(m_closedList.data(), m_closedList.size(), (*m_currentNeighbor));
		m_stats.timeFind += m_stats.clockBench.getElapsedTime();
#else
		m_find = FindNode(m_closedList.data(), m_closedList.size(), (*m_currentNeighbor));
#endif

		if (m_find == nullptr)
		{
#ifdef BENCH
			m_stats.clockBench.restart();
			(*m_currentNeighbor)->f = (*m_currentNeighbor)->g + DistanceHeuristic((*m_currentNeighbor), _goal);
			m_stats.timeHeuristic += m_stats.clockBench.restart();

			m_find = FindNode(m_openList.data(), m_openList.size(), (*m_currentNeighbor));
			m_stats.timeFind += m_stats.clockBench.getElapsedTime();
#else
			(*m_currentNeighbor)->f = (*m_currentNeighbor)->g + DistanceHeuristic((*m_currentNeighbor), _goal);
			m_find = FindNode(m_openList.data(), m_openList.size(), (*m_currentNeighbor));
#endif

			if (m_find == nullptr)
			{
				m_openList.push_back((*m_currentNeighbor));
			}
			else if ((*m_currentNeighbor)->g < (*m_find)->g)
			{
				(*m_find)->g = (*m_currentNeighbor)->g;
				(*m_find)->previous = (*m_currentNeighbor)->previous;
#ifdef BENCH
				m_stats.clockBench.restart();
				m_nodesPool.FreeObject((*m_currentNeighbor));
				m_stats.timeFreeObject += m_stats.clockBench.getElapsedTime();
			}
			else
			{
				m_stats.clockBench.restart();
				m_nodesPool.FreeObject((*m_currentNeighbor));
				m_stats.timeFreeObject += m_stats.clockBench.getElapsedTime();
			}
		}
		else
		{
			m_stats.clockBench.restart();
			m_nodesPool.FreeObject((*m_currentNeighbor));
			m_stats.timeFreeObject += m_stats.clockBench.getElapsedTime();
		}
#else
				m_nodesPool.FreeObject((*m_currentNeighbor));
			}
			else
				m_nodesPool.FreeObject((*m_currentNeighbor));
		}
		else
			m_nodesPool.FreeObject((*m_currentNeighbor));
#endif
	}
}

#ifdef DRAW_DEBUG
void AStar::CreateDebugShapes()
{
	sf::RectangleShape * rect;
	size_t openCount = m_openList.size();
	size_t closedCount = m_closedList.size();

	m_shapesPool.FreeAllObjects();
	m_debugShapes.clear();

	for (size_t i = 0; i < openCount; i++)
	{
		rect = m_shapesPool.GetObject();// new sf::RectangleShape({ m_scale, m_scale });
		rect->setSize({ m_scale, m_scale });
		rect->setFillColor(sf::Color::Green);
		rect->setPosition(static_cast<sf::Vector2f>(m_openList[i]->position) * m_scale);
		m_debugShapes.push_back(rect);
	}

	for (size_t i = 0; i < closedCount; i++)
	{
		rect = m_shapesPool.GetObject();// new sf::RectangleShape({ m_scale, m_scale });
		rect->setSize({ m_scale, m_scale });
		rect->setFillColor(sf::Color::Red);
		rect->setPosition(static_cast<sf::Vector2f>(m_closedList[i]->position) * m_scale);
		m_debugShapes.push_back(rect);
	}

	Node * current = m_start;
	while (current != m_goal)
	{
		rect = m_shapesPool.GetObject();// new sf::RectangleShape({ m_scale, m_scale });
		rect->setSize({ m_scale, m_scale });
		rect->setFillColor(sf::Color::Blue);
		rect->setPosition(static_cast<sf::Vector2f>(current->position) * m_scale);
		m_debugShapes.push_back(rect);
		current = current->next;
	}
}
#endif

AStar::Node ** AStar::FindNode(Node ** _first, size_t _size, Node * _value)
{
	for (; _size > 0; _size--, _first++)
	{
		if ((*_first)->position == _value->position)
			return _first;
	}

	return nullptr;
}