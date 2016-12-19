#include "Player.h"

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>

#include <SFML\Graphics\RenderTarget.hpp>

#include "Shot\SimpleShot.h"
#include "Shot\DoubleShot.h"

void Player::Init(char* _name, sf::Vector2f _position, float _shotSpeed, float _shotVelocity, float _movingSpeed, ShotInterface::ShotType _shotType, BulletsManager* _bulletsMng)
{
	m_isShooting = false;

	m_shotSpeed = 1 / _shotSpeed;
	m_shotVelocity = _shotVelocity;
	m_shotType = _shotType;
	m_movingSpeed = _movingSpeed;

	m_timeSinceLastShot = 0.0f;
	m_shotDirection.y = -1.0f;
	m_shotAngle = 0.0f;
	m_circleShotAngle = 0.0f;

	m_bulletsMng = _bulletsMng;

	m_sprite.setTexture(nep::TextureMng.GetTexture(_name));

	m_boundsSize = (sf::Vector2f)m_sprite.getTexture()->getSize();
	sf::Vector2f position = _position - (m_boundsSize / 2.0f);

	m_spriteCenter = (m_boundsSize / 2.0f) + position;
	m_sprite.setPosition(position);
	m_bounds.setPosition(position);

	m_bounds.setSize(m_boundsSize);
	m_bounds.setFillColor(sf::Color::Transparent);
	m_bounds.setOutlineColor(sf::Color::White);
	m_bounds.setOutlineThickness(1);

	m_shots[ShotInterface::Shot_Simple] = new SimpleShot(m_bulletsMng);
	m_shots[ShotInterface::Shot_Double] = new DoubleShot(m_bulletsMng, 10.f);
}


Player::~Player()
{
}

//---------------------------------------------------------------------------
// Méthodes de gestion des événements
// Souris
void Player::OnMouseMoved(sf::Event::MouseMoveEvent _mouseEvent)
{
	sf::Vector2f mousePos;
	mousePos.x = (float)_mouseEvent.x;
	mousePos.y = (float)_mouseEvent.y;
	
	m_targetPosition = mousePos;
}

void Player::OnMouseButtonPressed(sf::Event::MouseButtonEvent _mouseEvent)
{
	sf::Vector2f mousePos;
	mousePos.x = (float)_mouseEvent.x;
	mousePos.y = (float)_mouseEvent.y;

	m_isShooting = true;
}

void Player::OnMouseButtonReleased(sf::Event::MouseButtonEvent _mouseEvent)
{
	m_isShooting = false;
	m_shotAngle = 0.0f;
	m_circleShotAngle = 0.0f;
}

// Clavier
void Player::OnKeyPressed(sf::Event::KeyEvent _keyEvent)
{
	if (_keyEvent.code == sf::Keyboard::Num1)
		m_shotType = ShotInterface::Shot_Simple;
	else if (_keyEvent.code == sf::Keyboard::Num2)
		m_shotType = ShotInterface::Shot_Double;
}

//---------------------------------------------------------------------------
void Player::Update(float _deltaTime)
{
	m_timeSinceLastShot += _deltaTime;
	if (m_isShooting && m_timeSinceLastShot >= m_shotSpeed)
	{
		m_shots[m_shotType]->Fire(m_spriteCenter, m_shotDirection, m_shotVelocity);
		//for (int i = 0; i < 500; i++)
		//SpriralShot(10.0f);
		//SpriralShotAroundCircle(5.f, 0.5f, 100.0f);
		//CirlceShotAroundCircle(30.f, 20.f, 2.f, 200.f);

		m_timeSinceLastShot = 0;
	}

	m_moveDirection = m_targetPosition - m_spriteCenter;

	if ((m_moveDirection.x < -2.f || 2.f < m_moveDirection.x)
		|| (m_moveDirection.y < -2.f || 2.f < m_moveDirection.y))
	{
		nep::VectorNormalize(m_moveDirection);
		m_moveDirection = m_moveDirection * m_movingSpeed * _deltaTime;
		m_moveDirection.x = m_moveDirection.x;
		m_moveDirection.y = m_moveDirection.y;
		m_sprite.move(m_moveDirection);
		m_bounds.move(m_moveDirection);

		m_spriteCenter += m_moveDirection;
	}

	m_bulletsMng->Update(_deltaTime);
}

void Player::Draw(sf::RenderTarget& _target)
{
	_target.draw(*this);

	m_bulletsMng->Draw(_target);
}

void Player::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_target.draw(m_sprite, _states);

	if (m_debugMode)
		_target.draw(m_bounds, _states);
}

void Player::SpriralShot(float _angle)
{
	float cosAngle = cos(m_shotAngle);
	float sinAngle = sin(m_shotAngle);
	sf::Vector2f shotDirection;
	shotDirection.x = m_shotDirection.x * cosAngle - m_shotDirection.y * sinAngle;
	shotDirection.y = m_shotDirection.x * sinAngle + m_shotDirection.y * cosAngle;
	nep::VectorNormalize(shotDirection);
	m_bulletsMng->AddBullet(m_spriteCenter, shotDirection, m_shotVelocity);

	m_shotAngle += _angle * (nep::PI / 180);
}

void Player::SpriralShotAroundCircle(float _spiralAngle, float _circleAngle, float _circleRadius)
{
	float cosAngle = cos(m_shotAngle);
	float sinAngle = sin(m_shotAngle);

	sf::Vector2f shotDirection;
	shotDirection.x = m_shotDirection.x * cosAngle - m_shotDirection.y * sinAngle;
	shotDirection.y = m_shotDirection.x * sinAngle + m_shotDirection.y * cosAngle;
	nep::VectorNormalize(shotDirection);

	cosAngle = cos(m_circleShotAngle);
	sinAngle = sin(m_circleShotAngle);

	sf::Vector2f shotCirclePosition;
	shotCirclePosition.x = m_shotDirection.x * cosAngle - m_shotDirection.y * sinAngle;
	shotCirclePosition.y = m_shotDirection.x * sinAngle + m_shotDirection.y * cosAngle;
	nep::VectorNormalize(shotCirclePosition);
	shotCirclePosition *= _circleRadius;

	m_bulletsMng->AddBullet(m_spriteCenter + shotCirclePosition, shotDirection, m_shotVelocity);

	m_shotAngle += _spiralAngle * (nep::PI / 180);
	m_circleShotAngle += _circleAngle * (nep::PI / 180);
}

void Player::CirlceShotAroundCircle(float _c1Angle, float _c1Radius, float _c2Angle, float _c2Radius)
{
	float cosAngle = cos(m_shotAngle);
	float sinAngle = sin(m_shotAngle);

	sf::Vector2f shotDirection;
	shotDirection.x = m_shotDirection.x * cosAngle - m_shotDirection.y * sinAngle;
	shotDirection.y = m_shotDirection.x * sinAngle + m_shotDirection.y * cosAngle;
	nep::VectorNormalize(shotDirection);

	cosAngle = cos(m_circleShotAngle);
	sinAngle = sin(m_circleShotAngle);

	sf::Vector2f shotCirclePosition;
	shotCirclePosition.x = m_shotDirection.x * cosAngle - m_shotDirection.y * sinAngle;
	shotCirclePosition.y = m_shotDirection.x * sinAngle + m_shotDirection.y * cosAngle;
	nep::VectorNormalize(shotCirclePosition);
	shotCirclePosition *= _c2Radius;
	shotCirclePosition += (shotDirection * _c1Radius);

	m_bulletsMng->AddBullet(m_spriteCenter + shotCirclePosition, shotDirection, m_shotVelocity);

	m_shotAngle += _c1Angle * (nep::PI / 180);
	m_circleShotAngle += _c2Angle * (nep::PI / 180);
}