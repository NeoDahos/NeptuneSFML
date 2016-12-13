#include "FFTABattle.h"
#include "../FFTACharacter.h"

void FFTABattle::Init(sf::RenderWindow & _window)
{
	m_map.Init("../Data/Sprites/FFTA/Maps/Aisen Field.json", _window);

	const std::vector<sf::Vector2i> & startTiles = m_map.GetStarTiles();

	for (size_t i = 0; i < startTiles.size(); i++)
	{
		FFTAHighligthedTile highligthTile;
		highligthTile.Init(&m_map, startTiles[i]);
		m_highligthedTiles.push_back(std::move(highligthTile));
	}

	FFTACharacter* character;
	for (int i = 0; i < 2; i++)
	{
		character = new FFTACharacter;
		character->Init(&m_map, "../Data/Sprites/FFTA/Characters/Humans/Humans_Black Mage.json");
		character->SetPosition(-1, -1);
		m_characters.push_back(character);
	}

	m_cursor.Init(&m_map);
	m_characterInfos.Init(_window);

	m_state = BattleState::SelectDispatchCharacter;
	m_cursor.SetActive();

	m_selectedCharacterIndex = 0;
	m_cursor.SetPosition(m_highligthedTiles[0].GetTilePosition());
	m_selectedCharacter = m_characters[m_selectedCharacterIndex];
	m_selectedCharacter->m_isBeingDispatched = true;
	m_selectedCharacter->SetPosition(m_cursor.GetTilePosition());
	m_characterInfos.SetCharacter(m_selectedCharacter);
}

void FFTABattle::DeInit()
{
	m_map.DeInit();

	for (size_t i = 0; i < m_characters.size(); i++)
		delete m_characters[i];

	m_characters.clear();
	m_highligthedTiles.clear();
}

void FFTABattle::HandleEvent(sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (m_state == BattleState::SelectDispatchCharacter)
			HandleEvent_SelectDispatchCharacter(_event);
		else if (m_state == BattleState::DispatchCharacter)
			HandleEvent_DispatchCharacter(_event);
		else if (m_state == BattleState::OrientDispatchCharacter)
			HandleEvent_OrientDispatchCharacter(_event);
		else if (m_state == BattleState::ViewMode)
			HandleEvent_ViewMode(_event);
	}
}

void FFTABattle::Update(float _deltaTime)
{
	size_t i;
	m_map.Update(_deltaTime);
	m_cursor.Update(_deltaTime);

	for (i = 0; i < m_highligthedTiles.size(); i++)
		m_highligthedTiles[i].Update(_deltaTime);
	
	for (i = 0; i < m_characters.size(); i++)
		m_characters[i]->Update(_deltaTime);

	std::sort(m_characters.begin(), m_characters.end(), FFTABattle::SortCharacters);
	for (i = 0; i < m_characters.size(); i++)
	{
		if (m_characters[i] == m_selectedCharacter)
			m_selectedCharacterIndex = i;
	}

	m_characterInfos.Update(_deltaTime);
}

void FFTABattle::Draw(sf::RenderTarget & _target)
{
	size_t charIndex = 0;

	for (int layer = 0; layer <= m_map.GetLayerCount(); layer++)
	{
		m_map.DrawLayer(_target, layer);

		for (size_t j = 0; j < m_highligthedTiles.size(); j++)
		{
			if(m_highligthedTiles[j].GetDrawPriority() == layer)
				m_highligthedTiles[j].Draw(_target);
		}

		if (m_cursor.GetDrawPriority() == layer)
			m_cursor.DrawBottom(_target);

		for (; charIndex < m_characters.size(); charIndex++)
		{
			if (m_characters[charIndex]->GetDrawPriority() != layer)
				break;
			m_characters[charIndex]->Draw(_target);
		}
	}

	for (; charIndex < m_characters.size(); charIndex++)
		m_characters[charIndex]->Draw(_target);

	m_cursor.DrawTop(_target);

	switch (m_state)
	{
	case FFTABattle::SelectDispatchCharacter:
	case FFTABattle::DispatchCharacter:
		m_characterInfos.DrawInfosDispatch(_target);
		break;
	case FFTABattle::SelectCharacter:
		m_characterInfos.DrawInfosSelected(_target);
		break;
	default:
		break;
	}
}

bool FFTABattle::SortCharacters(FFTACharacter * _left, FFTACharacter * _rigth)
{
	sf::Vector2i leftTilePosition = _left->GetTilePosition();
	sf::Vector2i rigthTilePosition = _rigth->GetTilePosition();
	int leftDrawPriority = _left->GetDrawPriority();
	int rigthDrawPriority = _rigth->GetDrawPriority();

	return (leftDrawPriority < rigthDrawPriority)
		|| (leftDrawPriority == rigthDrawPriority && leftTilePosition.y < rigthTilePosition.y)
		|| (leftDrawPriority == rigthDrawPriority && leftTilePosition.y == rigthTilePosition.y && leftTilePosition.x < rigthTilePosition.x);
}

void FFTABattle::HandleEvent_SelectDispatchCharacter(sf::Event & _event)
{
	if (_event.key.code == sf::Keyboard::Left)
	{
		size_t newSelectedIndex = m_selectedCharacterIndex - 1;
		if (newSelectedIndex == -1)
			newSelectedIndex = m_characters.size() - 1;

		while (newSelectedIndex != m_selectedCharacterIndex && m_characters[newSelectedIndex]->m_isDispatched)
		{
			newSelectedIndex--;
			if (newSelectedIndex == -1)
				newSelectedIndex = m_characters.size() - 1;
		}

		if (newSelectedIndex != m_selectedCharacterIndex)
		{
			m_selectedCharacter->m_isBeingDispatched = false;
			m_selectedCharacterIndex--;
			if (m_selectedCharacterIndex == -1)
				m_selectedCharacterIndex = m_characters.size() - 1;

			size_t i = 0;
			while (i < m_highligthedTiles.size() && !m_highligthedTiles[i].GetIsActive())
				i++;

			m_cursor.SetPosition(m_highligthedTiles[i].GetTilePosition());
			m_selectedCharacter = m_characters[m_selectedCharacterIndex];
			m_selectedCharacter->m_isBeingDispatched = true;
			m_selectedCharacter->SetPosition(m_cursor.GetTilePosition());
			m_characterInfos.SetCharacter(m_selectedCharacter);
		}
	}
	else if (_event.key.code == sf::Keyboard::Right)
	{
		size_t newSelectedIndex = m_selectedCharacterIndex + 1;
		if (newSelectedIndex == m_characters.size())
			newSelectedIndex = 0;

		while (newSelectedIndex != m_selectedCharacterIndex && m_characters[newSelectedIndex]->m_isDispatched)
		{
			newSelectedIndex--;
			if (newSelectedIndex == -1)
				newSelectedIndex = m_characters.size() - 1;
		}

		if (newSelectedIndex != m_selectedCharacterIndex)
		{
			m_selectedCharacter->m_isBeingDispatched = false;
			m_selectedCharacterIndex++;
			if (m_selectedCharacterIndex == m_characters.size())
				m_selectedCharacterIndex = 0;

			size_t i = 0;
			while (i < m_highligthedTiles.size() && !m_highligthedTiles[i].GetIsActive())
				i++;

			m_cursor.SetPosition(m_highligthedTiles[i].GetTilePosition());
			m_selectedCharacter = m_characters[m_selectedCharacterIndex];
			m_selectedCharacter->m_isBeingDispatched = true;
			m_selectedCharacter->SetPosition(m_cursor.GetTilePosition());
			m_characterInfos.SetCharacter(m_selectedCharacter);
		}
	}
	else if (_event.key.code == sf::Keyboard::Space)
		m_state = BattleState::DispatchCharacter;
}

void FFTABattle::HandleEvent_DispatchCharacter(sf::Event & _event)
{
	if (_event.key.code == sf::Keyboard::Up)
		m_cursor.MoveInHighlightedTile(0, -1, m_highligthedTiles);
	else if (_event.key.code == sf::Keyboard::Down)
		m_cursor.MoveInHighlightedTile(0, 1, m_highligthedTiles);
	else if (_event.key.code == sf::Keyboard::Left)
		m_cursor.MoveInHighlightedTile(-1, 0, m_highligthedTiles);
	else if (_event.key.code == sf::Keyboard::Right)
		m_cursor.MoveInHighlightedTile(1, 0, m_highligthedTiles);
	else if (_event.key.code == sf::Keyboard::Space)
		m_state = BattleState::OrientDispatchCharacter;

	m_selectedCharacter->SetPosition(m_cursor.GetTilePosition());
}

void FFTABattle::HandleEvent_OrientDispatchCharacter(sf::Event & _event)
{
	if (_event.key.code == sf::Keyboard::Up)
		m_selectedCharacter->SetDirection(FFTACharacter::Direction::TR);
	else if (_event.key.code == sf::Keyboard::Down)
		m_selectedCharacter->SetDirection(FFTACharacter::Direction::BL);
	else if (_event.key.code == sf::Keyboard::Left)
		m_selectedCharacter->SetDirection(FFTACharacter::Direction::TL);
	else if (_event.key.code == sf::Keyboard::Right)
		m_selectedCharacter->SetDirection(FFTACharacter::Direction::BR);
	else if (_event.key.code == sf::Keyboard::Space)
	{
		bool isCursorPlaced = false;
		m_selectedCharacter->m_isBeingDispatched = false;
		m_selectedCharacter->m_isDispatched = true;

		for (size_t i = 0; i < m_highligthedTiles.size(); i++)
		{
			if (m_highligthedTiles[i].GetTilePosition() == m_selectedCharacter->GetTilePosition())
				m_highligthedTiles[i].SetActive(false);

			if (m_highligthedTiles[i].GetIsActive() && !isCursorPlaced)
			{
				m_cursor.SetPosition(m_highligthedTiles[i].GetTilePosition());
				isCursorPlaced = true;
			}
		}

		size_t newSelectedIndex = m_selectedCharacterIndex + 1;
		if (newSelectedIndex == m_characters.size())
			newSelectedIndex = 0;

		while (newSelectedIndex != m_selectedCharacterIndex && m_characters[newSelectedIndex]->m_isDispatched)
		{
			newSelectedIndex++;
			if (newSelectedIndex == m_characters.size())
				newSelectedIndex = 0;
		}

		if (newSelectedIndex != m_selectedCharacterIndex)
		{
			m_selectedCharacterIndex = newSelectedIndex;
			m_selectedCharacter = m_characters[m_selectedCharacterIndex];
			m_selectedCharacter->m_isBeingDispatched = true;
			m_selectedCharacter->SetPosition(m_cursor.GetTilePosition());
			m_characterInfos.SetCharacter(m_selectedCharacter);
			m_state = BattleState::SelectDispatchCharacter;
		}
		else
		{
			m_selectedCharacter = nullptr;
			m_state = BattleState::ViewMode;
			m_highligthedTiles.clear();
		}
	}
}

void FFTABattle::HandleEvent_ViewMode(sf::Event & _event)
{
	if (_event.key.code == sf::Keyboard::Up)
		m_cursor.Move(0, -1);
	else if (_event.key.code == sf::Keyboard::Down)
		m_cursor.Move(0, 1);
	else if (_event.key.code == sf::Keyboard::Left)
		m_cursor.Move(-1, 0);
	else if (_event.key.code == sf::Keyboard::Right)
		m_cursor.Move(1, 0);
}