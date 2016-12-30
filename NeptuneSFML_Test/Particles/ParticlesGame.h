#ifndef PARTICLES_GAME_H
#define PARTICLES_GAME_H
#include <NeptuneSFML\Game.h>

#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\Emitter.h>
#include <NeptuneSFML\Particles\Effectors\Repeller.h>
#include <NeptuneSFML\Particles\Effectors\ColorByVelocity.h>
#include <NeptuneSFML\Particles\Effectors\LoopZone.h>

#include <NeptuneSFML\UI\Button.h>
#include <NeptuneSFML\UI\Slider.h>

class ParticlesGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	void ApplyBtn_OnClick(sf::Event::MouseButtonEvent _buttonEvent);
	void Sliders_OnValueChange(float _newValue);
	void EmitterTypeSdr_OnValueChange(float _newValue);

	nep::Emitter m_emitter;
	nep::Repeller m_repeller;
	nep::ColorByVelocity m_colorByVelocity;
	nep::LoopZone m_loopZone;
	nep::VertexParticleSystem m_vertexParticleSystem;
	sf::RenderWindow * m_window;

	// UI
	std::vector<nep::Widget *> m_widgets;
	nep::Button m_applyBtn;
	nep::Slider m_particleCountSdr;
	nep::Slider m_emitterTypeSdr;
	nep::Slider m_emitterSpawnRateSdr;
	nep::Slider m_emitterPulseRateSdr;
	nep::Slider m_emitterQuantitySdr;
};

#endif //PARTICLES_GAME_H