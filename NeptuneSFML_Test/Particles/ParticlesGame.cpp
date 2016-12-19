#include "ParticlesGame.h"

#include <NeptuneSFML\Particles\VertexParticle.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>
#include <NeptuneSFML\Object\SpriteBatch.h>

void ParticlesGame::Init(sf::RenderWindow& _window)
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(_window.getSize());
	nep::SpriteBatch::GetInstance();
	m_vertexParticleSystem.Init(windowSize / 2.f);

	m_emitter.Init(&m_vertexParticleSystem, 15.f, 15.f);
	m_emitter.SetTypeContinuous(0.001f);
	m_emitter.SetShapeLine({ -100.f, 0.f }, { 100.f, 0.f });
	m_vertexParticleSystem.AddEmitter(&m_emitter);

	m_repeller.Init(static_cast<sf::Vector2f>(_window.getSize()) / 2.f, -15.f, false);
	m_vertexParticleSystem.AddEffector(&m_repeller);

	m_colorByVelocity.Init(sf::Color(255, 255, 0, 255), sf::Color(255, 0, 0, 255), 0.f, 100.f);
	m_vertexParticleSystem.AddEffector(&m_colorByVelocity);

	m_loopZone.Init(sf::FloatRect({ 0.f, 0.f }, windowSize));
	m_vertexParticleSystem.AddEffector(&m_loopZone);
}

void ParticlesGame::DeInit()
{
}

void ParticlesGame::HandleEvent(sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			printf("Particle count : %d\n", m_vertexParticleSystem.GetParticleCount());
		}
	}
	else if (_event.type == sf::Event::MouseButtonPressed)
	{
		m_repeller.setPosition(static_cast<float>(_event.mouseButton.x), static_cast<float>(_event.mouseButton.y));
		m_repeller.Activate();
	}
	else if (_event.type == sf::Event::MouseButtonReleased)
	{
		m_repeller.Activate(false);
	}
	else if (_event.type == sf::Event::MouseMoved)
	{
		m_repeller.setPosition(static_cast<float>(_event.mouseMove.x), static_cast<float>(_event.mouseMove.y));
	}
}

void ParticlesGame::Update(float _deltaTime)
{	
	m_vertexParticleSystem.Update(_deltaTime);
}

void ParticlesGame::Draw(sf::RenderTarget& _target)
{
	m_vertexParticleSystem.Draw(_target);
}