#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include "Shot\ShotInterface.h"
#include "BulletsManager.h"

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Player : public sf::Drawable
{
public:
	// Constructeurs/Desctructeurs
	Player() {};
	virtual ~Player();

	void Init(char* _name, sf::Vector2f _position, float _shotSpeed, float _shotVelocity, float _movingSpeed, ShotInterface::ShotType _shotType, BulletsManager* _bulletsMng);
	// Accesseurs/Mutateurs

	// Méthodes de gestion des événements
	// Souris
	void OnMouseButtonPressed(sf::Event::MouseButtonEvent _mouseEvent);
	void OnMouseButtonReleased(sf::Event::MouseButtonEvent _mouseEvent);
	void OnMouseMoved(sf::Event::MouseMoveEvent _mouseEvent);

	// Clavier
	void OnKeyPressed(sf::Event::KeyEvent _keyEvent);

	// Autres
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget& _target);

private:
	virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states = sf::RenderStates::Default) const;

	// Bullets pattern
	void SpriralShot(float _angle);
	void SpriralShotAroundCircle(float _spiralAngle, float _circleAngle, float _circleRadius);
	void CirlceShotAroundCircle(float _c1Angle, float _c1Radius, float _c2Angle, float _c2Radius);

//Attributs
public:
	bool m_debugMode = false;
	sf::Sprite m_sprite;

private:
	// Shot infos
	bool m_isShooting;
	float m_shotSpeed; // Time between each shot
	float m_shotVelocity;
	float m_timeSinceLastShot;
	ShotInterface::ShotType m_shotType;
	ShotInterface* m_shots[ShotInterface::ShotType_Count];

	float m_shotAngle;
	float m_circleShotAngle;
	sf::Vector2f m_shotDirection;

	BulletsManager* m_bulletsMng;

	// Move infos
	float m_movingSpeed;
	sf::Vector2f m_targetPosition;
	sf::Vector2f m_moveDirection;

	sf::Vector2f m_spriteCenter;
	sf::RectangleShape m_bounds;
	sf::Vector2f m_boundsSize;
};

