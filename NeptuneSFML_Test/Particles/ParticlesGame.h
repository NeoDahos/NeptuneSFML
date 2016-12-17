#ifndef PARTICLES_GAME_H
#define PARTICLES_GAME_H
#include <NeptuneSFML\Game.h>

#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Particles\Effectors\ColorByVelocity.h>
#include <NeptuneSFML\Particles\Effectors\LoopZone.h>

class ParticlesGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	nep::Repeller m_repeller;
	nep::ColorByVelocity m_colorByVelocity;
	nep::LoopZone m_loopZone;
	nep::VertexParticleSystem m_vertexParticleSystem;
};

#endif //PARTICLES_GAME_H