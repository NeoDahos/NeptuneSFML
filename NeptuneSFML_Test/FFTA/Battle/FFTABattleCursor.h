#ifndef FFTA_BATTLE_CURSOR_H
#define FFTA_BATTLE_CURSOR_H

#include <NeptuneSFML\Animation\Animation.h>

#include <SFML\Graphics\RenderTarget.hpp>

#include "FFTAHighligthedTile.h"

class FFTAMap;

class FFTABattleCursor
{
public:
	void Init(FFTAMap * _map);

	void Update(float _deltaTime);
	void DrawBottom(sf::RenderTarget & _target);
	void DrawTop(sf::RenderTarget & _target);

	void SetPosition(int _x, int _y);
	void SetPosition(const sf::Vector2i & _position);
	sf::Vector2i GetTilePosition() const;
	void Move(int _x, int _y);
	void MoveInHighlightedTile(int _x, int _y, const std::vector<FFTAHighligthedTile> & _highligthedTiles);
	int GetDrawPriority() const;
	void SetActive(bool _active = true);

private:
	nep::Animation m_cursorTop;
	sf::Sprite m_cursorBottom;
	sf::Vector2i m_tilePosition;
	FFTAMap * m_map;
	int m_drawPriority;
	bool m_isActive;
};

#endif // FFTA_BATTLE_CURSOR_H