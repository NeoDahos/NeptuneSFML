#include "SoftBodyObject.h"

#include <NeptuneSFML\Tools.h>

#include <SFML\Graphics\VertexArray.hpp>

sf::FloatRect SoftBodyObject::s_bounds;
float SoftBodyObject::s_gravity = 100.f;
bool SoftBodyObject::s_drawVelocities = false;
bool SoftBodyObject::s_drawNormals = false;

void SoftBodyObject::Init(const std::vector<Point>& _points, float _mass, float _pressure, float _springElasticity, float _springDamping)
{
	m_points = _points;
	m_mass = _mass;
	m_springElasticity = _springElasticity;
	m_springDamping = _springDamping;

	const size_t pointCount = m_points.size();
	for (size_t i = 0; i < pointCount - 1; i++)
		AddSpring(i, i + 1);

	AddSpring(pointCount - 1, 0);

	for (int i = 0; i < 100; i++)
	{
		m_pressure += _pressure / 100.f;
		for (size_t i = 0; i < m_points.size(); i++)
		{
			m_points[i].forceAccumulator.x = 0.f;
			m_points[i].forceAccumulator.y = 0.f;
		}

		ComputeSpringForce();
		ComputePressureForce();
		IntegrateEuler(0.016f);
	}
}

void SoftBodyObject::Update(float _deltaTime)
{
	ComputeGravityForce();
	ComputeSpringForce();
	ComputePressureForce();

	IntegrateEuler(_deltaTime);
}

void SoftBodyObject::Draw(sf::RenderTarget & _target)
{
	//sf::VertexArray pointsVertices(sf::PrimitiveType::Points, m_points.size());
	sf::VertexArray springsVertices(sf::PrimitiveType::Lines, (m_springs.size()) * 2);
	sf::VertexArray normalsVertices(sf::PrimitiveType::Lines, m_springs.size() * 2);
	sf::VertexArray velocitiesVertices(sf::PrimitiveType::Lines, m_points.size() * 2);
	size_t i;

	if (s_drawVelocities)
	{
		for (i = 0; i < m_points.size(); i++)
		{
			//pointsVertices[i].position = m_points[i].position;
			//pointsVertices[i].color = sf::Color::Red;

			velocitiesVertices[(i * 2)].position = m_points[i].position;
			velocitiesVertices[(i * 2)].color = sf::Color::Red;
			velocitiesVertices[(i * 2) + 1].position = m_points[i].position + (m_points[i].velocity / 10.f);
			velocitiesVertices[(i * 2) + 1].color = sf::Color::Red;
		}
	}

	for (size_t i = 0; i < m_springs.size(); i++)
	{
		springsVertices[(i * 2)].position = m_points[m_springs[i].start].position;
		springsVertices[(i * 2)].color = sf::Color::Blue;
		springsVertices[(i * 2) + 1].position = m_points[m_springs[i].end].position;
		springsVertices[(i * 2) + 1].color = sf::Color::Blue;

		if (s_drawNormals)
		{
			normalsVertices[(i * 2)].position = m_points[m_springs[i].start].position;
			normalsVertices[(i * 2)].color = sf::Color::Green;
			normalsVertices[(i * 2) + 1].position = m_points[m_springs[i].start].position + (m_springs[i].normalVector * 10.f);
			normalsVertices[(i * 2) + 1].color = sf::Color::Green;
		}
	}

	_target.draw(springsVertices);
	//_target.draw(pointsVertices);

	if (s_drawNormals)
		_target.draw(normalsVertices);

	if (s_drawVelocities)
		_target.draw(velocitiesVertices);
}

void SoftBodyObject::AddSpring(int _startIndex, int _endIndex)
{
	Spring spring;
	spring.start = _startIndex;
	spring.end = _endIndex;
	spring.length = nep::VectorLength(m_points[_endIndex].position - m_points[_startIndex].position);
	m_springs.push_back(std::move(spring));
}

void SoftBodyObject::ComputeGravityForce()
{
	for (size_t i = 0; i < m_points.size(); i++)
	{
		m_points[i].forceAccumulator.x = 0.f;
		m_points[i].forceAccumulator.y = m_mass * s_gravity;
	}
}

void SoftBodyObject::ComputeSpringForce()
{
	sf::Vector2f endToStart;
	sf::Vector2f forceVector; // force vector
	sf::Vector2f velocity; // velocity of start & end points
	float endToStartLength; // length of p1 - p2 vector
	sf::Vector2f hookeForce; // hooke force value

	for (size_t i = 0; i < m_springs.size(); i++)
	{
		// get vector of spring from start to end points
		endToStart = m_points[m_springs[i].start].position - m_points[m_springs[i].end].position;

		// calculate sqr(distance)
		endToStartLength = nep::VectorLength(endToStart);

		if (endToStartLength != 0) // start = end?
		{
			// get velocities of start & end points
			velocity = m_points[m_springs[i].start].velocity - m_points[m_springs[i].end].velocity;

			// calculate force value
			//hookeForce = m_springElasticity * (endToStartLength - m_springs[i].length) + m_springDamping * nep::DotProduct(velocity, (endToStart / endToStartLength));
			hookeForce.x = m_springElasticity * (endToStartLength - m_springs[i].length) + m_springDamping * (velocity.x * (endToStart / endToStartLength).x);
			hookeForce.y = m_springElasticity * (endToStartLength - m_springs[i].length) + m_springDamping * (velocity.y * (endToStart / endToStartLength).y);

			// force vector
			forceVector.x = (endToStart / endToStartLength).x * hookeForce.x;
			forceVector.y = (endToStart / endToStartLength).y * hookeForce.y;

			// accumulate force for starting point
			m_points[m_springs[i].start].forceAccumulator -= forceVector;

			// accumulate force for end point
			m_points[m_springs[i].end].forceAccumulator += forceVector;
		}

		// Calculate normal vectors to springs
		m_springs[i].normalVector.x = endToStart.y / endToStartLength;
		m_springs[i].normalVector.y = -endToStart.x / endToStartLength;
	}
}

void SoftBodyObject::ComputePressureForce()
{
	sf::Vector2f endToStart;
	sf::Vector2f objectCenter;
	sf::Vector2f startToCenter;
	float endToStartLength; // length of p1 - p2 vector
	float volume = 0.f;
	float pressureForce; // pressure force value
	size_t i = 0;

	for (i = 0; i < m_points.size(); i++)
		objectCenter += m_points[i].position;
	objectCenter /= (float)m_points.size();

	for (i = 0; i < m_points.size() - 1; i++)
	{
		// get vector of spring from start to end points
		endToStart = m_points[i].position - m_points[i + 1].position;

		// calculate sqr(distance)
		endToStartLength = nep::VectorLength(endToStart);

		startToCenter = objectCenter - m_points[i].position;
		volume += 0.5f * endToStartLength * nep::VectorLength(m_springs[i].normalVector * nep::DotProduct(startToCenter, m_springs[i].normalVector));
	}

	// get vector of spring from start to end points
	endToStart = m_points[i].position - m_points[0].position;

	// calculate sqr(distance)
	endToStartLength = nep::VectorLength(endToStart);

	startToCenter = objectCenter - m_points[i].position;
	volume += 0.5f * endToStartLength * nep::VectorLength(m_springs[i].normalVector * nep::DotProduct(startToCenter, m_springs[i].normalVector));

	for (i = 0; i < m_springs.size(); i++)
	{
		// get vector of spring from start to end points
		endToStart = m_points[m_springs[i].start].position - m_points[m_springs[i].end].position;

		// calculate sqr(distance)
		endToStartLength = nep::VectorLength(endToStart);

		pressureForce = (endToStartLength * m_pressure) / volume;

		m_points[m_springs[i].start].forceAccumulator += m_springs[i].normalVector * pressureForce;
		m_points[m_springs[i].end].forceAccumulator += m_springs[i].normalVector * pressureForce;
	}
}

void SoftBodyObject::IntegrateEuler(float _deltaTime)
{
	sf::Vector2f dr; // dr for Euler integration

	/* Euler Integrator (second Newton's law) */
	for (size_t i = 0; i < m_points.size(); i++)
	{
		/* x */
		m_points[i].velocity.x += (m_points[i].forceAccumulator.x / m_mass) * _deltaTime;
		dr.x = m_points[i].velocity.x * _deltaTime;

		/* y */
		m_points[i].velocity.y += (m_points[i].forceAccumulator.y / m_mass) * _deltaTime;
		dr.y = m_points[i].velocity.y * _deltaTime;

		/* Boundaries  X */
		if (m_points[i].position.x + dr.x < s_bounds.left)
		{
			dr.x = s_bounds.left - m_points[i].position.x;
			m_points[i].velocity.x = 0.f;// -0.1f * m_points[i].velocity.x;
			//m_points[i].velocity.y = 0.95f * m_points[i].velocity.y;
		}
		else if (m_points[i].position.x + dr.x > s_bounds.left + s_bounds.width)
		{
			dr.x = (s_bounds.left + s_bounds.width) - m_points[i].position.x;
			m_points[i].velocity.x = 0.f;// -0.1f * m_points[i].velocity.x;
			//m_points[i].velocity.y = 0.95f * m_points[i].velocity.y;
		}

		/* Boundaries  Y */
		if (m_points[i].position.y + dr.y < s_bounds.top)
		{
			dr.y = s_bounds.left - m_points[i].position.y;
			m_points[i].velocity.y = 0.f;// -0.1f * m_points[i].velocity.y;
			//m_points[i].velocity.x = 0.95f * m_points[i].velocity.x;
		}
		else if (m_points[i].position.y + dr.y > s_bounds.top + s_bounds.height)
		{
			dr.y = s_bounds.top + s_bounds.height - m_points[i].position.y;
			m_points[i].velocity.y = 0.f;// -0.1f * m_points[i].velocity.y;
			//m_points[i].velocity.x = 0.95f * m_points[i].velocity.x;
		}

		m_points[i].position += dr;
	}
}