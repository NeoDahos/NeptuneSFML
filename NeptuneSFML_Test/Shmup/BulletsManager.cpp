#include "BulletsManager.h"
#include "Bullet.h"

#include <SFML\Graphics\RenderTarget.hpp>

#include <NeptuneSFML\Object\SpriteBatch.h>
#include <NeptuneSFML\EngineCore.h>

void BulletsManager::Init()
{
	m_bulletTexture = &nep::TextureMng.GetTexture("bullet");
	m_gameTerrain = { -50, -50, 800.f + 50.0f, 600.f + 50.0f };

	Bullet * bullet= nullptr;
	for (unsigned short i = 0; i < s_poolSize; i++)
	{
		bullet = m_bulletPool.GetObject();
		bullet->setTexture(*m_bulletTexture);
	}

	m_bulletPool.FreeAllObjects();
}

// Autres
void BulletsManager::AddBullet(const sf::Vector2f& _position, const sf::Vector2f& _direction, float _speed, const sf::Color& _color)
{
	if (m_bullets.size() == s_poolSize)
		return;

	Bullet * bullet = m_bulletPool.GetObject();
	if (bullet != nullptr)
	{
		bullet->Init(_position, _direction, _speed, _color);
		m_bullets.push_back(bullet);
	}
}

void BulletsManager::Update(float _deltaTime)
{
	for (int i = m_bullets.size() - 1; i >= 0; i--)
	{
		m_bullets[i]->Update(_deltaTime);

		if (!m_gameTerrain.contains(m_bullets[i]->getPosition()))
		{
			m_bulletPool.FreeObject(m_bullets[i]);
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
	}
}

void BulletsManager::Draw(sf::RenderTarget& _target)
{
	nep::SpriteBatch::GetInstance().Draw<Bullet>(_target, m_bullets, m_bulletTexture);
}
