#ifndef FFTA_CHARACTER_INFOS_H
#define FFTA_CHARACTER_INFOS_H

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include <NeptuneSFML\Animation\Animation.h>

class FFTACharacter;

class FFTACharacterInfos
{
public:
	void Init(sf::RenderWindow & _window);
	void Update(float _deltaTime);
	void DrawInfosSelected(sf::RenderTarget & _target);
	void DrawInfosDispatch(sf::RenderTarget & _target);

	void SetCharacter(FFTACharacter * _character);

private:
	// Selected character
	sf::Sprite m_selectedBackground;
	sf::Sprite m_selectedForeground;
	sf::Vector2f m_selectedPosition;

	// Place character
	sf::Sprite m_placeBackground;
	sf::Sprite m_placeForeground;
	sf::Sprite m_placePedestalBackground;
	nep::Animation m_placePedestalCharacter;
	sf::Vector2f m_placePosition;
	sf::Vector2f m_placePedestalPosition;

	// Common
	sf::Sprite m_portait;
	FFTACharacter * m_character;
};

#endif // FFTA_CHARACTER_INFOS_H