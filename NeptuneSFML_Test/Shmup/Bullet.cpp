#include "Bullet.h"

#include <NeptuneSFML\EngineCore.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>

Bullet::Bullet(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color)
{
	m_direction = _direction;
	m_speed = _speed;
	m_color = _color;

	setTexture(nep::TextureMng.GetTexture("bullet"));
	setColor(m_color);

	sf::Vector2f size = (sf::Vector2f)getTexture()->getSize();
	setPosition(_position - (size / 2.0f));
}


Bullet::~Bullet()
{
}

void Bullet::Init(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color)
{
	m_direction = _direction;
	m_speed = _speed;
	m_color = _color;

	setColor(m_color);

	sf::Vector2f size = (sf::Vector2f)getTexture()->getSize();
	setPosition(_position - (size / 2.0f));
}

void Bullet::Update(float _deltaTime)
{
	move(m_direction * m_speed * _deltaTime);
}