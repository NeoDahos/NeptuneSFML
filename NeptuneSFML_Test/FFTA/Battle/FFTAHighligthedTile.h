#ifndef FFTA_HIGHLIGTHED_TILE_H
#define FFTA_HIGHLIGTHED_TILE_H

#include <NeptuneSFML\Animation\Animation.h>

class FFTAMap;

class FFTAHighligthedTile
{
public:
	void Init(FFTAMap * _map, const sf::Vector2i & _tilePosition);
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget & _target);

	int GetDrawPriority() const;
	sf::Vector2i GetTilePosition() const;
	void SetActive(bool _active = true);
	bool GetIsActive() const;

private:
	nep::Animation m_blueTileAnim;
	sf::Vector2i m_tilePosition;
	int m_drawPriority;
	bool m_isActive;
};

#endif // FFTA_HIGHLIGTHED_TILE_H