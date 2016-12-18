#include "ParticlesGame.h"

#include <NeptuneSFML\Particles\VertexParticle.h>
#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>
#include <NeptuneSFML\Object\SpriteBatch.h>

void ParticlesGame::Init(sf::RenderWindow& _window)
{
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(_window.getSize());
	nep::SpriteBatch::GetInstance();
	m_vertexParticleSystem.Init(windowSize / 2.f, windowSize);
	//nep::TextureMng.AddTexture("particle", "../Data/Sprites/Particles/particle.png");
	//m_spriteParticleSystem.Init(windowSize / 2.f, "particle");

	for (int i = 0; i < 300000; i++)
	{
		m_vertexParticleSystem.AddParticle(sf::Vector2f(cos(DegToRad(i % 36000)) * (rand() % 16), sin(DegToRad(i % 36000)) * (rand() % 16)));
		//m_spriteParticleSystem.AddParticle(sf::Vector2f(cos(DegToRad(i % 36000)) * (rand() % 32), sin(DegToRad(i % 36000)) * (rand() % 32)));
	}

	m_repeller.Init(static_cast<sf::Vector2f>(_window.getSize()) / 2.f, -18.f, false);
	m_vertexParticleSystem.AddEffector(&m_repeller);
	//m_spriteParticleSystem.AddEffector(&m_repeller);

	m_colorByVelocity.Init(sf::Color(255, 255, 0, 255), sf::Color(255, 0, 0, 255), 0.f, 50.f);
	m_vertexParticleSystem.AddEffector(&m_colorByVelocity);
	//m_spriteParticleSystem.AddEffector(&m_colorByVelocity);

	m_loopZone.Init(sf::FloatRect({ 0.f, 0.f }, windowSize));
	m_vertexParticleSystem.AddEffector(&m_loopZone);
	//m_spriteParticleSystem.AddEffector(&m_loopZone);
}

void ParticlesGame::DeInit()
{
	//nep::TextureMng.EraseTexture("particle");
}

void ParticlesGame::HandleEvent(sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			printf("Particle count : %d\n", m_vertexParticleSystem.GetParticleCount());
			//printf("Particle count : %d\n", m_spriteParticleSystem.GetParticleCount());
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
	//m_spriteParticleSystem.Update(_deltaTime);
}

void ParticlesGame::Draw(sf::RenderTarget& _target)
{
	m_vertexParticleSystem.Draw(_target);
	//m_spriteParticleSystem.Draw(_target);
}