#pragma once

#include <NeptuneSFML\Object\ObjectPool.h>

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Color.hpp>

#include <vector>

#include "Bullet.h"

namespace sf
{
	class RenderTarget;
}

class BulletsManager
{
// Méthodes
public:
	void Init();
	void AddBullet(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color = sf::Color::White);

	void Update(float _deltaTime);
	void Draw(sf::RenderTarget& _target);

// Attributs
private:
	static const unsigned short s_poolSize = 1000;
	nep::ObjectPool<Bullet, s_poolSize> m_bulletPool;
	std::vector<Bullet*> m_bullets;

	sf::Texture * m_bulletTexture;
	sf::FloatRect m_gameTerrain;
};
