#include "ParticlesGame.h"

#include <NeptuneSFML\Particles\VertexParticle.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>

void ParticlesGame::Init(sf::RenderWindow& _window)
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(_window.getSize());

	m_window = &_window;

	m_vertexParticleSystem.Init(windowSize / 2.f, 100000);

	m_emitter.Init(&m_vertexParticleSystem, 5.f, 15.f);
	m_emitter.SetTypeContinuous(0.001f);
	//m_emitter.SetTypePulse(0.5f, 10000);
	m_emitter.SetShapePoint({ 0.f, 0.f });
	//m_emitter.SetShapeCircle({ 0.f, 0.f }, 50.f);
	//m_emitter.SetShapeLine({ -100.f, 50.f }, { 100.f, -50.f });
	m_vertexParticleSystem.AddEmitter(&m_emitter);

	m_repeller.Init(static_cast<sf::Vector2f>(_window.getSize()) / 2.f, -15.f, false);
	m_vertexParticleSystem.AddEffector(&m_repeller);

	m_colorByVelocity.Init(sf::Color(255, 255, 0, 255), sf::Color(255, 0, 0, 255), 0.f, 100.f);
	m_vertexParticleSystem.AddEffector(&m_colorByVelocity);

	m_loopZone.Init(sf::FloatRect({ 0.f, 0.f }, windowSize));
	m_vertexParticleSystem.AddEffector(&m_loopZone);

	m_button.SetRenderTarger(m_window);
	m_button.SetPosition({ 50.f, 100.f });
	m_button.SetOnClickFct(std::bind(&ParticlesGame::ButtonOnClick, this, std::placeholders::_1));

	m_slider.SetRenderTarger(m_window);
	m_slider.SetPosition({50.f, 170.f});
	m_slider.SetOnValueChangeFct(std::bind(&ParticlesGame::SliderOnValueChange, this, std::placeholders::_1));
}

void ParticlesGame::DeInit()
{
}

void ParticlesGame::HandleEvent(const sf::Event & _event)
{
	bool isEventConsumed = m_button.HandleEvent(_event);
	if (isEventConsumed)
		m_slider.HandleEvent(_event);
	else
		isEventConsumed = m_slider.HandleEvent(_event);

	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			printf("Particle count : %d\n", m_vertexParticleSystem.GetParticleCount());
		}
	}
	else if (_event.type == sf::Event::MouseButtonPressed && !isEventConsumed)
	{
		m_repeller.SetPosition(m_window->mapPixelToCoords({ _event.mouseButton.x, _event.mouseButton.y }));
		m_repeller.SetActive();
	}
	else if (_event.type == sf::Event::MouseButtonReleased)
	{
		m_repeller.SetActive(false);
	}
	else if (_event.type == sf::Event::MouseMoved)
	{
		m_repeller.SetPosition(m_window->mapPixelToCoords({ _event.mouseMove.x, _event.mouseMove.y }));
	}
}

void ParticlesGame::Update(float _deltaTime)
{	
	m_vertexParticleSystem.Update(_deltaTime);
}

void ParticlesGame::Draw(sf::RenderTarget& _target)
{
	m_vertexParticleSystem.Draw(_target);

	m_button.Draw(_target);
	m_slider.Draw(_target);
}

void ParticlesGame::ButtonOnClick(sf::Event::MouseButtonEvent _buttonEvent)
{
	printf("Click !\n");
	m_slider.SetBigStep(25.f);
	m_slider.SetMinValue(25.f);
}

void ParticlesGame::SliderOnValueChange(float _newValue)
{
	printf("Slider value : %f\n", _newValue);
}