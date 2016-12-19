#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\VertexParticle.h>

#include <SFML\System\Sleep.hpp>

namespace nep
{
	VertexParticleSystem::VertexParticleSystem() : m_vertices(sf::PrimitiveType::Points),
		m_threadDataCopy(m_vertices, m_particles, m_effectors, ThreadData<4>::CopyToVertexArray), m_threadDataUpdate(m_vertices, m_particles, m_effectors, ThreadData<4>::UpdateParticle)
	{
	}


	VertexParticleSystem::~VertexParticleSystem()
	{
		m_threadDataCopy.Stop();
		m_threadDataUpdate.Stop();

		int particleCount = static_cast<int>(m_particles.size());
		for (int i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void VertexParticleSystem::Init(const sf::Vector2f& _position)
	{
		m_position = _position;
	}

	void VertexParticleSystem::Update(float _deltaTime)
	{
		size_t effectorCount = m_effectors.size();
		const int particleCount = static_cast<int>(m_particles.size());
		const int emitterCount = static_cast<int>(m_emitters.size());
		sf::Vector2f newPosition;
		m_threadDataUpdate.deltaTime = _deltaTime;

		for (int i = 0; i < emitterCount; i++)
			m_emitters[i]->Update(_deltaTime);

		m_threadDataUpdate.Process();
		while (m_threadDataUpdate.IsProcessing())
			sf::sleep(sf::microseconds(100));

		for (int i = particleCount - 1; i >= 0; i--)
		{
			if (m_particles[i]->IsDead())
			{
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
		}
	}

	void VertexParticleSystem::Draw(sf::RenderTarget & _target)
	{
		int particleCount = static_cast<int>(m_particles.size());

		if (particleCount > 0)
		{
			m_vertices.resize(particleCount);

			m_threadDataCopy.Process();
			while (m_threadDataCopy.IsProcessing())
				sf::sleep(sf::microseconds(100));

			_target.draw(m_vertices);
		}
	}

	void VertexParticleSystem::AddParticle(const sf::Vector2f & _position, const sf::Vector2f & _initialForce, float _mass, const sf::Color & _color)
	{
		VertexParticle* newParticle = new VertexParticle();
		newParticle->Init(m_position + _position, _initialForce, _mass);
		newParticle->color = _color;
		m_particles.push_back(newParticle);
	}

	void VertexParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	size_t VertexParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}