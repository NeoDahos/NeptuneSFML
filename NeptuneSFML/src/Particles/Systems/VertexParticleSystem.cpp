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

		KillAllParticles();
	}

	void VertexParticleSystem::Init(const sf::Vector2f& _position, int _maxParticleCount, bool _startActive)
	{
		m_position = _position;
		m_maxParticleCount = _maxParticleCount;
		m_isActive = _startActive;
	}

	void VertexParticleSystem::Update(float _deltaTime)
	{
		if (!m_isActive)
			return;

		size_t effectorCount = m_effectors.size();
		sf::Vector2f newPosition;
		m_threadDataUpdate.deltaTime = _deltaTime;

		for (int i = 0; i < static_cast<int>(m_emitters.size()); i++)
			m_emitters[i]->Update(_deltaTime);

		m_threadDataUpdate.Process();
		while (m_threadDataUpdate.IsProcessing())
			sf::sleep(sf::microseconds(10));

		for (int i = static_cast<int>(m_particles.size()) - 1; i >= 0; i--)
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
		if (!m_isActive)
			return;

		size_t particleCount = m_particles.size();
		if (particleCount > 0)
		{
			m_vertices.resize(particleCount);

			m_threadDataCopy.Process();
			while (m_threadDataCopy.IsProcessing())
				sf::sleep(sf::microseconds(10));

			_target.draw(m_vertices);
		}
	}

	bool VertexParticleSystem::AddParticle(const sf::Vector2f & _position, const sf::Vector2f & _initialForce, float _mass, const sf::Color & _color)
	{
		if (m_isActive && m_particles.size() < m_maxParticleCount)
		{
			VertexParticle* newParticle = new VertexParticle();
			newParticle->Init(m_position + _position, _initialForce, _mass);
			newParticle->color = _color;
			m_particles.push_back(newParticle);
			return true;
		}

		return false;
	}

	void VertexParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	void VertexParticleSystem::KillAllParticles()
	{
		int particleCount = static_cast<int>(m_particles.size());
		for (int i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	size_t VertexParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}