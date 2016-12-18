#ifndef SPRITE_PARTICLE_SYSTEM_H
#define SPRITE_PARTICLE_SYSTEM_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\String.hpp>

#include <vector>

namespace nep
{
	class SpriteParticle;
	class ParticleEffector;

	class NEPTUNE_API SpriteParticleSystem
	{
	public:
		SpriteParticleSystem() {}
		~SpriteParticleSystem();

		void Init(const sf::Vector2f& _position, const sf::String& _texturename);
		void Update(float _deltaTime);
		void Draw(sf::RenderTarget& _target);

		void AddParticle(sf::Vector2f _initialForce = sf::Vector2f(), float _mass = 1.f);
		void AddEffector(ParticleEffector * const _effector);

		void AddForce(sf::Vector2f _force);

		size_t GetParticleCount() const;

	private:
		std::vector<SpriteParticle*> m_particles;
		std::vector<ParticleEffector *> m_effectors;
		sf::Vector2f m_position;
		const sf::Texture* m_texture;
	};
}

#endif // SPRITE_PARTICLE_SYSTEM_H