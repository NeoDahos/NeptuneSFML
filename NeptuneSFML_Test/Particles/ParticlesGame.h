#ifndef PARTICLES_GAME_H
#define PARTICLES_GAME_H
#include <NeptuneSFML\Game.h>

#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\Emitter.h>
#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Particles\Effectors\ColorByVelocity.h>
#include <NeptuneSFML\Particles\Effectors\LoopZone.h>

#include <NeptuneSFML\UI\Button.h>

class ParticlesGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	void ButtonOnClick(sf::Event::MouseButtonEvent _buttonEvent);

	nep::Emitter m_emitter;
	nep::Repeller m_repeller;
	nep::ColorByVelocity m_colorByVelocity;
	nep::LoopZone m_loopZone;
	nep::VertexParticleSystem m_vertexParticleSystem;
	nep::Button m_button;
	sf::RenderWindow * m_window;
};

#endif //PARTICLES_GAME_H