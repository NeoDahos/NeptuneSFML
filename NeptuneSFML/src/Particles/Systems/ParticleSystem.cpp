#include <NeptuneSFML\Particles\Systems\ParticleSystem.h>

namespace nep
{
	ParticleSystem::~ParticleSystem()
	{
		size_t particleCount = m_particles.size() - 1;
		for (size_t i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void ParticleSystem::Init(const sf::Vector2f & _position)
	{
		m_position = _position;
	}

	void ParticleSystem::Update(float _deltaTime)
	{
		int particleCount = static_cast<int>(m_particles.size());

		for (int i = particleCount - 1; i >= 0; i--)
		{
			m_particles[i]->Update(_deltaTime);
			if (m_particles[i]->IsDead())
			{
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
		}
	}

	void ParticleSystem::Draw(sf::RenderTarget & _target)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->Draw(_target);
	}

	void ParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	size_t ParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}