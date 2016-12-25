#include "..\..\include\NeptuneSFML\Particles\Emitter.h"
#include <NeptuneSFML\Particles\Emitter.h>
#include <NeptuneSFML\Particles\Systems\ParticleSystem.h>
#include <NeptuneSFML\Tools.h>

namespace nep
{
	void Emitter::Init(ParticleSystem * _particleSystem, float _minInitialForce, float _maxInitialForce, bool _startActive)
	{
		m_particleSystem = _particleSystem;
		m_minInitialForce = _minInitialForce;
		m_maxInitialForce = _maxInitialForce;
		m_shape = EmitterShape::Point;
		m_type = EmitterType::Continuous;
		m_timeSinceLastEmission = 0.f;
		m_shapeData.point.position.x = 0.f;
		m_shapeData.point.position.y = 0.f;
		m_typeData.spawnRate = 1.f;
		m_isActive = _startActive;
	}

	void Emitter::Update(float _deltaTime)
	{
		if (!m_isActive)
			return;

		m_timeSinceLastEmission += _deltaTime;
		if (m_type == EmitterType::Continuous)
		{
			while (m_timeSinceLastEmission >= m_typeData.spawnRate)
			{
				m_timeSinceLastEmission -= m_typeData.spawnRate;
				if (m_shape == EmitterShape::Point)
				{
					if (!EmitPoint(1))
						break;
				}
				else if (m_shape == EmitterShape::Line)
				{
					if (!EmitLine(1))
						break;
				}
				else if (m_shape == EmitterShape::Circle)
				{
					if (!EmitCircle(1))
						break;
				}
			}
		}
		else if (m_type == EmitterType::Pulse)
		{
			if (m_timeSinceLastEmission >= m_typeData.pulseRate)
			{
				m_timeSinceLastEmission = 0.f;

				if (m_shape == EmitterShape::Point)
					EmitPoint(m_typeData.quantity);
				else if (m_shape == EmitterShape::Line)
					EmitLine(m_typeData.quantity);
				else if (m_shape == EmitterShape::Circle)
					EmitCircle(m_typeData.quantity);
			}
		}
	}

	void Emitter::SetTypeContinuous(float _spawnRate)
	{
		m_type = EmitterType::Continuous;
		m_typeData.spawnRate = _spawnRate;
	}

	void Emitter::SetTypePulse(float _pulseRate, int _quantity)
	{
		m_type = EmitterType::Pulse;
		m_typeData.pulseRate = _pulseRate;
		m_typeData.quantity = _quantity;
		m_timeSinceLastEmission = _pulseRate;
	}

	void Emitter::SetShapePoint(const sf::Vector2f & _position)
	{
		m_shape = EmitterShape::Point;
		m_shapeData.point.position = _position;
	}

	void Emitter::SetShapeLine(const sf::Vector2f & _lineStart, const sf::Vector2f & _lineEnd)
	{
		m_shape = EmitterShape::Line;
		m_shapeData.line.start = _lineStart;
		m_shapeData.line.end = _lineEnd;
	}

	void Emitter::SetShapeCircle(const sf::Vector2f & _center, float _radius)
	{
		m_shape = EmitterShape::Circle;
		m_shapeData.circle.center = _center;
		m_shapeData.circle.radius = _radius;
	}

	void Emitter::SetActive(bool _activate)
	{
		m_isActive = _activate;
	}

	bool Emitter::EmitPoint(int _count)
	{
		std::uniform_real_distribution<float> randForce(m_minInitialForce, m_maxInitialForce);
		std::uniform_real_distribution<float> randAngle(0.f, TWO_PI);
		float angle;
		float force;

		for (int i = 0; i < _count; i++)
		{
			force = randForce(m_randGenerator);
			angle = randAngle(m_randGenerator);
			if (!m_particleSystem->AddParticle(m_shapeData.point.position, { cos(angle) * force, sin(angle) * force }))
				return false;
		}

		return true;
	}

	bool Emitter::EmitLine(int _count)
	{
		sf::Vector2f line = m_shapeData.line.end - m_shapeData.line.start;
		sf::Vector2f linePerpendicular;
		const float lineLength = VectorLength(line);
		std::uniform_real_distribution<float> randPosition(0.0f, lineLength);
		std::uniform_real_distribution<float> randForce(m_minInitialForce, m_maxInitialForce);
		float force;

		VectorNormalize(line);
		linePerpendicular = line;
		VectorRotation(linePerpendicular, PI2);

		for (int i = 0; i < _count; i++)
		{
			force = randForce(m_randGenerator);
			if (!m_particleSystem->AddParticle(m_shapeData.line.start + line * randPosition(m_randGenerator), linePerpendicular * force))
				return false;
		}

		return true;
	}

	bool Emitter::EmitCircle(int _count)
	{
		std::uniform_real_distribution<float> randForce(m_minInitialForce, m_maxInitialForce);
		std::uniform_real_distribution<float> randAngle(0.f, TWO_PI);
		sf::Vector2f position;
		float angle;
		float force;

		for (int i = 0; i < _count; i++)
		{
			force = randForce(m_randGenerator);
			angle = randAngle(m_randGenerator);
			position.x = m_shapeData.circle.center.x + cos(angle) * m_shapeData.circle.radius;
			position.y = m_shapeData.circle.center.x + sin(angle) * m_shapeData.circle.radius;

			if (!m_particleSystem->AddParticle(position, { cos(angle) * force, sin(angle) * force }))
				return false;
		}

		return true;
	}
}