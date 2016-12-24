#include "ShmupGame.h"

void ShmupGame::Init(sf::RenderWindow& _window)
{
	nep::TextureMng.AddTexture("ship", "../Data/Sprites/Shmup/ship.png");
	nep::TextureMng.AddTexture("bullet", "../Data/Sprites/Shmup/bullet.png");

	m_player.Init("ship", static_cast<sf::Vector2f>(_window.getSize()) / 2.f, 5.f, 500.f, 200.f, ShotInterface::ShotType::Shot_Simple, &m_bulletsManager);
	m_bulletsManager.Init();
}

void ShmupGame::DeInit()
{
	nep::TextureMng.EraseTexture("ship");
	nep::TextureMng.EraseTexture("bullet");
}

void ShmupGame::HandleEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::MouseButtonPressed)
		m_player.OnMouseButtonPressed(_event.mouseButton);
	else if (_event.type == sf::Event::MouseButtonReleased)
		m_player.OnMouseButtonReleased(_event.mouseButton);
	else if (_event.type == sf::Event::MouseMoved)
		m_player.OnMouseMoved(_event.mouseMove);
	else if (_event.type == sf::Event::KeyPressed)
		m_player.OnKeyPressed(_event.key);
}

void ShmupGame::Update(float _deltaTime)
{
	m_player.Update(_deltaTime);
}

void ShmupGame::Draw(sf::RenderTarget& _target)
{
	m_player.Draw(_target);
}