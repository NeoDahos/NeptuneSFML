#include "FFTAHighligthedTile.h"
#include "FFTAMap.h"

#include <NeptuneSFML\EngineCore.h>

#include <SFML\Graphics\RenderTarget.hpp>

void FFTAHighligthedTile::Init(FFTAMap * _map, const sf::Vector2i & _tilePosition)
{
	m_blueTileAnim.Init(nep::TextureMng.GetTexture("TileHighlightBlue"), 1, 7, 7, 0.2f);
	m_tilePosition = _tilePosition;

	m_blueTileAnim.setPosition(_map->GetIsometricPosition(m_tilePosition));
	m_blueTileAnim.setOrigin(m_blueTileAnim.getLocalBounds().width / 2.f, m_blueTileAnim.getLocalBounds().height / 2.f);
	m_drawPriority = _map->GetTilePriority(m_tilePosition);
	m_isActive = true;
}

void FFTAHighligthedTile::Update(float _deltaTime)
{
	if(m_isActive)
		m_blueTileAnim.Update(_deltaTime);
}

void FFTAHighligthedTile::Draw(sf::RenderTarget & _target)
{
	if(m_isActive)
		_target.draw(m_blueTileAnim);
}

int FFTAHighligthedTile::GetDrawPriority() const
{
	return m_drawPriority;
}

sf::Vector2i FFTAHighligthedTile::GetTilePosition() const
{
	return m_tilePosition;
}

void FFTAHighligthedTile::SetActive(bool _active)
{
	m_isActive = _active;
	if (m_isActive)
		m_blueTileAnim.Play();
	else
		m_blueTileAnim.Stop();
}

bool FFTAHighligthedTile::GetIsActive() const
{
	return m_isActive;
}
