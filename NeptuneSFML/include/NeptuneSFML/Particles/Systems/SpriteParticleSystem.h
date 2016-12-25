#ifndef SPRITE_PARTICLE_SYSTEM_H
#define SPRITE_PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Systems\ParticleSystem.h>

#include <SFML\System\String.hpp>

namespace nep
{
	class SpriteParticle;
	class ParticleEffector;

	class NEPTUNE_API SpriteParticleSystem : public ParticleSystem
	{
	public:
		SpriteParticleSystem() {}
		virtual ~SpriteParticleSystem();

		void Init(const sf::Vector2f& _position, const sf::String& _texturename, bool _startActive = true);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderTarget& _target);

		virtual bool AddParticle(const sf::Vector2f & _position = sf::Vector2f(), const sf::Vector2f & _initialForce = sf::Vector2f(), float _mass = 1.f, const sf::Color & _color = sf::Color::White);
		virtual void AddForce(sf::Vector2f _force);
		virtual void KillAllParticles();

		virtual size_t GetParticleCount() const;

	private:
		std::vector<SpriteParticle*> m_particles;
		const sf::Texture* m_texture;
	};
}

#endif // SPRITE_PARTICLE_SYSTEM_H