#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Sprite.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Bullet : public sf::Sprite
{
// Méthodes
public:
	// Constructeurs/Desctructeurs
	Bullet() {};
	Bullet(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White);
	~Bullet();

	void Init(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White);
	
	// Autres
	virtual void Update(float _deltaTime);

// Attributs
private:
	sf::Vector2f m_direction;
	sf::Color m_color;
	float m_speed = 0.f;
};

