#include <NeptuneSFML\Particles\SpriteParticle.h>

#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	void SpriteParticle::Init(const sf::Vector2f& _position, sf::Vector2f _initialForce, float _mass, const sf::String& _texturename)
	{
		Particle::Init(_position, _initialForce, _mass);
		setPosition(_position);

		if(!_texturename.isEmpty())
			setTexture(TextureMng.GetTexture(_texturename));
	}

	void SpriteParticle::Update(float _deltaTime)
	{
		Particle::Update(_deltaTime);
		setPosition(Particle::m_position);
	}

	void SpriteParticle::Draw(sf::RenderTarget& _target)
	{
		_target.draw(*this);
	}
}