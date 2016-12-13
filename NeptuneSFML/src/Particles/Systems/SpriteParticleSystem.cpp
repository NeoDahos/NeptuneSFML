#include <NeptuneSFML\Particles\Systems\SpriteParticleSystem.h>
#include <NeptuneSFML\Particles\SpriteParticle.h>
#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	SpriteParticleSystem::~SpriteParticleSystem()
	{
		size_t particleCount = m_particles.size() - 1;
		for (size_t i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void SpriteParticleSystem::Init(const sf::Vector2f & _position, const sf::String& _texturename)
	{
		m_position = _position;
		m_texture = &TextureMng.GetTexture(_texturename);
		m_spriteBatch.SetTexture(m_texture);
	}

	void SpriteParticleSystem::Update(float _deltaTime)
	{
		int particleCount = static_cast<int>(m_particles.size());

		for (int i = particleCount - 1; i >= 0; i--)
		{
			m_particles[i]->Update(_deltaTime);
			if (m_particles[i]->IsDead())
			{
				m_spriteBatch.RemoveSprite(m_particles[i]);
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
		}
	}

	void SpriteParticleSystem::Draw(sf::RenderTarget & _target)
	{
		m_spriteBatch.Draw(_target);
	}

	void SpriteParticleSystem::AddParticle(sf::Vector2f _initialForce, float _mass)
	{
		SpriteParticle* newParticle = new SpriteParticle();
		newParticle->Init(m_position, _initialForce, _mass);
		newParticle->setTexture(*m_texture);
		m_particles.push_back(newParticle);
		m_spriteBatch.AddSprite(newParticle);
	}

	void SpriteParticleSystem::AddForce(sf::Vector2f _force)
	{
		size_t particleCount = m_particles.size();
		for (size_t i = 0; i < particleCount; i++)
			m_particles[i]->AddForce(_force);
	}

	size_t SpriteParticleSystem::GetParticleCount() const
	{
		return m_particles.size();
	}
}