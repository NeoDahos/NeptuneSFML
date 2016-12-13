#include "ParticlesGame.h"

#include <NeptuneSFML\Particles\VertexParticle.h>

#include <NeptuneSFML\EngineCore.h>

void ParticlesGame::Init(sf::RenderWindow& _window)
{
	m_gravity = { 0.f, 0.f };

	nep::TextureMng.AddTexture("particle", "../Data/Sprites/Particles/particle.png");

	m_vertexParticleSystem.Init(static_cast<sf::Vector2f>(_window.getSize()) / 2.f, static_cast<sf::Vector2f>(_window.getSize()));
	
	for (int i = 0; i < 50000; i++)
		m_vertexParticleSystem.AddParticle(sf::Vector2f(((rand() % 200) / 100.f - 1.f) * 3.f, ((rand() % 300) / 100.f - 2.f) * 3.f), 1.f);
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
			//for (int i = 0; i < 1000; i++)
				//m_vertexParticleSystem.AddParticle(sf::Vector2f(((rand() % 200) / 100.f - 1.f) * 30.f, ((rand() % 200) / 100.f - 1.f) * 30.f), 1.f);

			printf("Particle count : %d\n", m_vertexParticleSystem.GetParticleCount());
		}
	}
	else if (_event.type == sf::Event::MouseButtonPressed)
	{
		nep::Repeller repeller;
		repeller.Init({(float) _event.mouseButton.x, (float)_event.mouseButton.y }, -100000.f);
		m_repellers.push_back(repeller);
	}
	else if (_event.type == sf::Event::MouseButtonReleased)
	{
		m_repellers.clear();
	}
	else if (_event.type == sf::Event::MouseMoved)
	{
		if (!m_repellers.empty())
		{
			m_repellers[0].setPosition(static_cast<float>(_event.mouseMove.x), static_cast<float>(_event.mouseMove.y));
		}
	}
}

void ParticlesGame::Update(float _deltaTime)
{
	//m_vertexParticleSystem.AddParticle(sf::Vector2f(((rand() % 200) / 100.f - 1.f) * 30.f, ((rand() % 200) / 100.f - 1.f) * 30.f), 10.f);

	m_vertexParticleSystem.AddForce(m_gravity);

	size_t repellerCount = m_repellers.size();
	for (size_t i = 0; i < repellerCount; i++)
		m_vertexParticleSystem.AddRepellerForce(m_repellers[i]);

	m_vertexParticleSystem.Update(_deltaTime);
}

void ParticlesGame::Draw(sf::RenderTarget& _target)
{
	/*size_t repellerCount = m_repellers.size();
	for (size_t i = 0; i < repellerCount; i++)
		_target.draw(m_repellers[i]);*/

	m_vertexParticleSystem.Draw(_target);
}