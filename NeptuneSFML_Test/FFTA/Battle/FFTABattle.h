#ifndef FFTA_BATTLE_H
#define FFTA_BATTLE_H

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

#include "FFTABattleCursor.h"
#include "FFTAMap.h"
#include "FFTACharacterInfos.h"
#include "FFTAHighligthedTile.h"

class FFTACharacter;

class FFTABattle
{
public:
	void Init(sf::RenderWindow & _window);
	void DeInit();
	void HandleEvent(const sf::Event& _event);
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget & _target);

private:
	static bool SortCharacters(FFTACharacter * _left, FFTACharacter * _rigth);

	void HandleEvent_SelectDispatchCharacter(const sf::Event& _event);
	void HandleEvent_DispatchCharacter(const sf::Event& _event);
	void HandleEvent_OrientDispatchCharacter(const sf::Event& _event);
	void HandleEvent_ViewMode(const sf::Event& _event);

	enum BattleState {SelectDispatchCharacter = 0, DispatchCharacter, OrientDispatchCharacter, SelectCharacter, OrientCharacter, SelectAction, MoveCharacter, CharacterMoving,
		SelectAttackTarget, CharacterAttacking, ViewMode, BattleStateCount};

	std::vector<FFTAHighligthedTile> m_highligthedTiles;
	std::vector<FFTACharacter *> m_characters;
	FFTABattleCursor m_cursor;
	FFTACharacterInfos m_characterInfos;
	FFTAMap m_map;
	BattleState m_state;
	FFTACharacter * m_selectedCharacter;
	size_t m_selectedCharacterIndex;
};

#endif // FFTA_BATTLE_H