#include "SonicGame.h"

#include <NeptuneSFML\EngineCore.h>

void SonicGame::Init(sf::RenderWindow& _window)
{
	nep::TextureMng.AddTexture("Sonic", "../Data/Sonic/Sonic 1/Sonic.png");
	nep::TextureMng.AddTexture("Green Hill Zone", "../Data/Sonic/Sonic 1/Green Hill Zone.png");

	m_sonic.Init("Sonic", {400.f, 580.f});

	m_level.Init("Green Hill Zone");

	Level::Chunk chunk;
	chunk.position = { 0.f, 344.f };
	chunk.rect = { 5, 2876, 256, 256};
	m_level.AddChunk(chunk);
}

void SonicGame::DeInit()
{
}

void SonicGame::HandleEvent(const sf::Event& _event)
{
}

void SonicGame::Update(float _deltaTime)
{
	m_sonic.Update(_deltaTime);
}

void SonicGame::Draw(sf::RenderTarget& _target)
{
	m_level.Draw(_target);
	m_sonic.Draw(_target);
}