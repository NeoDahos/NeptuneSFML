#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\VertexParticle.h>

#include <SFML\Graphics\VertexArray.hpp>

namespace nep
{
	VertexParticleSystem::~VertexParticleSystem()
	{
		int particleCount = static_cast<int>(m_particles.size());
		for (int i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void VertexParticleSystem::Init(const sf::Vector2f& _position, const sf::Vector2f& _windowSize)
	{
		m_position = _position;
		m_windowSize = _windowSize;
	}

	void VertexParticleSystem::Update(float _deltaTime)
	{
		int particleCount = static_cast<int>(m_particles.size());
		sf::Vector2f newPosition;

		for (int i = particleCount - 1; i >= 0; i--)
		{
			m_particles[i]->Update(_deltaTime);
			if (m_particles[i]->IsDead())
			{
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
			else
			{
				newPosition = m_particles[i]->position;
				if (newPosition.x < 0)
					newPosition.x += m_windowSize.x;
				else if (newPosition.x >= m_windowSize.x)
					newPosition.x -= m_windowSize.x;

				if (newPosition.y < 0)
					newPosition.y += m_windowSize.y;
				else if (newPosition.y >= m_windowSize.y)
					newPosition.y -= m_windowSize.y;

				m_particles[i]->SetPosition(newPosition);
			}
		}
	}

	void VertexParticleSystem::Draw(sf::RenderTarget & _target)
	{
		size_t particleCount = m_particles.size();

		if (particleCount > 0)
		{
			sf::VertexArray vertices(sf::PrimitiveType::Points);
			VertexParticle** currentParticle = m_particles.data();

			for (size_t i = 0; i < particleCount; i++)
			{
				vertices.append(**currentParticle);
				currentParticle++;
			}

			_target.draw(vertices);
		}
	}

	void VertexParticleSystem::AddParticle(sf::Vector2f _initialForce, float _mass, const sf::Color & _color)
	{
		VertexParticle* newParticle = new VertexParticle();
		newParticle->Init(m_position, _initialForce, _mass);
		m_particles.push_back(newParticle);
	}

	void VertexParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	void VertexParticleSystem::AddRepellerForce(Repeller& _repeller)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_repeller.ComputeRepelForce(m_particles[i]));
	}

	size_t VertexParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}