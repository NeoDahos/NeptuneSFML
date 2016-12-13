#include <NeptuneSFML\TiledMap\TiledMap.h>
#include <NeptuneSFML\TiledMap\TiledMapZone.h>

#include <SFML\Graphics\RenderTarget.hpp>

namespace nep
{
	void TiledMap::AddZone(const sf::String& _name, TiledMapZone* _zone)
	{
		if (m_zones.find(_name) != m_zones.end())
			return;

		m_zones[_name] = _zone;
	}

	void TiledMap::SetCurrentZone(const sf::String& _name)
	{
		if (m_zones.find(_name) == m_zones.end())
			return;

		m_currentZone = m_zones[_name];
	}

	void TiledMap::Update(float _deltaTime)
	{
		m_currentZone->Update(_deltaTime);
	}

	void TiledMap::Draw(sf::RenderTarget& _target)
	{
		m_currentZone->Draw(_target);
	}
}