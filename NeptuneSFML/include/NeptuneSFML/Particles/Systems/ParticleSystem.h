#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Particle.h>

#include <vector>

namespace nep
{
	class NEPTUNE_API ParticleSystem
	{
	public:
		ParticleSystem() {}
		~ParticleSystem();

		void Init(const sf::Vector2f& _position);
		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

		template<typename T>
		void AddParticle(sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::String& _texturename = "");

		void AddForce(sf::Vector2f _force);

		size_t GetParticleCount() const;

	private:
		std::vector<Particle*> m_particles;
		sf::Vector2f m_position;
	};

	template<typename T>
	void ParticleSystem::AddParticle(sf::Vector2f _initialForce, float _mass, const sf::String& _texturename)
	{
		T* newParticle = new T();
		newParticle->Init(m_position, _initialForce, _mass, _texturename);
		m_particles.push_back(newParticle);
	}
}

#endif // PARTICLE_SYSTEM_H