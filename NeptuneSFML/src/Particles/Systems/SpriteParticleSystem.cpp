#include <NeptuneSFML\Particles\Systems\SpriteParticleSystem.h>
#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>
#include <NeptuneSFML\Particles\SpriteParticle.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Object\SpriteBatch.h>

namespace nep
{
	SpriteParticleSystem::~SpriteParticleSystem()
	{
		int particleCount = m_particles.size();
		for (int i = 0; i < particleCount; i++)
			delete m_particles[i];

		m_particles.clear();
	}

	void SpriteParticleSystem::Init(const sf::Vector2f & _position, const sf::String& _texturename)
	{
		m_position = _position;
		m_texture = &TextureMng.GetTexture(_texturename);
	}

	void SpriteParticleSystem::Update(float _deltaTime)
	{
		const size_t effectorCount = m_effectors.size();
		const int particleCount = static_cast<int>(m_particles.size());
		const int emitterCount = static_cast<int>(m_emitters.size());

		for (int i = 0; i < emitterCount; i++)
			m_emitters[i]->Update(_deltaTime);

		for (int i = particleCount - 1; i >= 0; i--)
		{
			m_particles[i]->Update(_deltaTime);
			for (size_t j = 0; j < effectorCount; j++)
				m_effectors[j]->Apply(m_particles[i]);

			if (m_particles[i]->IsDead())
			{
				delete m_particles[i];
				m_particles[i] = m_particles.back();
				m_particles.pop_back();
			}
		}
	}

	void SpriteParticleSystem::Draw(sf::RenderTarget & _target)
	{
		SpriteBatch::GetInstance().Draw<SpriteParticle>(_target, m_particles, m_texture);
	}

	void SpriteParticleSystem::AddParticle(const sf::Vector2f & _position, const sf::Vector2f & _initialForce, float _mass, const sf::Color & _color)
	{
		SpriteParticle* newParticle = new SpriteParticle();
		newParticle->Init(m_position + _position, _initialForce, _mass);
		newParticle->setTexture(*m_texture);
		newParticle->setColor(_color);
		m_particles.push_back(newParticle);
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