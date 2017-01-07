#ifndef SOFT_BODY_OBJECT_H
#define SOFT_BODY_OBJECT_H

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include <vector>

class SoftBodyObject
{
public:
	struct Point
	{
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Vector2f forceAccumulator;
	};

	struct Spring
	{
		sf::Vector2f normalVector;
		float length; // rest length
		int start, end; // points indexes
	};

	void Init(const std::vector<Point> & _points, float _mass, float _pressure, float _springElasticity = 1500.f, float _springDamping = 18.f);
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget& _target);

	void AddSpring(int _startIndex, int _endIndex);

	static sf::FloatRect s_bounds;
	static float s_gravity;
	static bool s_drawVelocities;
	static bool s_drawNormals;

protected:
	void ComputeGravityForce();
	void ComputeSpringForce();
	void ComputePressureForce();
	void IntegrateEuler(float _deltaTime);

	std::vector<Point> m_points;
	std::vector<Spring> m_springs;
	float m_pressure;
	float m_mass;
	float m_springElasticity; // ?
	float m_springDamping; // ?
};

#endif // SOFT_BODY_OBJECT_H