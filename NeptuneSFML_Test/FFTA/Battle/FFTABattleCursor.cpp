#include "FFTABattleCursor.h"
#include "FFTAMap.h"

#include <NeptuneSFML\EngineCore.h>

void FFTABattleCursor::Init(FFTAMap * _map)
{
	const sf::Texture & cursorTexture = nep::TextureMng.GetTexture("BattleCursor");
	sf::Vector2f cursorBottomOrigin(16.f, 8.f);
	sf::Vector2f cursorTopOrigin(16.f, 45.f);

	m_cursorTop.Init(cursorTexture, { { 0, 0, 32, 37 },{ 32, 0, 32, 37 },{ 64, 0, 32, 37 },{ 96, 0, 32, 37 },{ 128, 0, 32, 37 },{ 160, 0, 32, 37 },{ 192, 0, 32, 37 },{ 224, 0, 32, 37 } },
		{ 0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.08f, 0.2f });
	m_cursorTop.setOrigin(cursorTopOrigin);
	m_cursorBottom.setTexture(cursorTexture);
	m_cursorBottom.setTextureRect({ 0, 37, 32, 16 });
	m_cursorBottom.setOrigin(cursorBottomOrigin);

	m_map = _map;
	m_drawPriority = 0;
}

void FFTABattleCursor::Update(float _deltaTime)
{
	if(m_isActive)
		m_cursorTop.Update(_deltaTime);
}

void FFTABattleCursor::DrawBottom(sf::RenderTarget & _target)
{
	if (m_isActive)
		_target.draw(m_cursorBottom);
}

void FFTABattleCursor::DrawTop(sf::RenderTarget & _target)
{
	if (m_isActive)
		_target.draw(m_cursorTop);
}

void FFTABattleCursor::SetPosition(int _x, int _y)
{
	m_tilePosition = m_map->GetClampedTilePosition(_x, _y);

	m_cursorTop.setPosition(m_map->GetIsometricPosition(m_tilePosition));
	m_cursorBottom.setPosition(m_map->GetIsometricPosition(m_tilePosition));

	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

void FFTABattleCursor::SetPosition(const sf::Vector2i & _position)
{
	m_tilePosition = m_map->GetClampedTilePosition(_position);

	m_cursorTop.setPosition(m_map->GetIsometricPosition(m_tilePosition));
	m_cursorBottom.setPosition(m_map->GetIsometricPosition(m_tilePosition));

	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

sf::Vector2i FFTABattleCursor::GetTilePosition() const
{
	return m_tilePosition;
}

void FFTABattleCursor::Move(int _x, int _y)
{
	if (!m_isActive)
		return;

	m_tilePosition.x += _x;
	m_tilePosition.y += _y;
	m_tilePosition = m_map->GetClampedTilePosition(m_tilePosition.x, m_tilePosition.y);

	m_cursorTop.setPosition(m_map->GetIsometricPosition(m_tilePosition));
	m_cursorBottom.setPosition(m_map->GetIsometricPosition(m_tilePosition));
	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

void FFTABattleCursor::MoveInHighlightedTile(int _x, int _y, const std::vector<FFTAHighligthedTile> & _highligthedTiles)
{
	if (!m_isActive)
		return;

	sf::Vector2i newPosition = m_map->GetClampedTilePosition(m_tilePosition.x + _x, m_tilePosition.y + _y);

	size_t tileCount = _highligthedTiles.size();
	size_t i = 0;
	while (i < tileCount && _highligthedTiles[i].GetTilePosition() != newPosition)
		i++;

	if (i != tileCount)
	{
		m_tilePosition = newPosition;
		m_cursorTop.setPosition(m_map->GetIsometricPosition(m_tilePosition));
		m_cursorBottom.setPosition(m_map->GetIsometricPosition(m_tilePosition));
		m_drawPriority = m_map->GetTilePriority(m_tilePosition);
	}
}

int FFTABattleCursor::GetDrawPriority() const
{
	return m_drawPriority;
}

void FFTABattleCursor::SetActive(bool _active)
{
	m_isActive = _active;

	if (m_isActive)
		m_cursorTop.Play();
	else
		m_cursorTop.Stop();
}
