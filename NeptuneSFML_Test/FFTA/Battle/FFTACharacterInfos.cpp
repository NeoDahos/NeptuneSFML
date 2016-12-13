#include "FFTACharacterInfos.h"
#include "../FFTACharacter.h"

#include <NeptuneSFML\EngineCore.h>

void FFTACharacterInfos::Init(sf::RenderWindow & _window)
{
	sf::Vector2f windowSize(_window.getSize());
	sf::Texture & texture = nep::TextureMng.GetTexture("SelectedCharacterInfos");

	// Selected character
	m_selectedPosition.y = windowSize.y - 3.f;

	m_selectedBackground.setTexture(texture);
	m_selectedBackground.setTextureRect({0, 53, 135, 53});
	m_selectedBackground.setPosition(0.f, m_selectedPosition.y - 53.f);

	m_selectedForeground.setTexture(texture);
	m_selectedForeground.setTextureRect({ 0, 0, 135, 53 });
	m_selectedForeground.setPosition(0.f, m_selectedPosition.y - 53.f);

	// Place character
	texture = nep::TextureMng.GetTexture("PlaceCharacterInfos");
	m_placePosition.y = windowSize.y - 3.f;
	m_placePedestalPosition.x = windowSize.x;
	m_placePedestalPosition.y = windowSize.y - 8.f;

	m_placeBackground.setTexture(texture);
	m_placeBackground.setTextureRect({ 0, 53, 135, 53 });
	m_placeBackground.setPosition(0.f, m_placePosition.y - 53.f);

	m_placeForeground.setTexture(texture);
	m_placeForeground.setTextureRect({ 0, 0, 135, 53 });
	m_placeForeground.setPosition(0.f, m_placePosition.y - 53.f);

	m_placePedestalBackground.setTexture(texture);
	m_placePedestalBackground.setTextureRect({ 135, 58, 64, 48 });
	m_placePedestalBackground.setPosition(m_placePedestalPosition.x - 64.f, m_placePedestalPosition.y - 48.f);
}

void FFTACharacterInfos::Update(float _deltaTime)
{
	m_placePedestalCharacter.Update(_deltaTime);
}

void FFTACharacterInfos::DrawInfosSelected(sf::RenderTarget & _target)
{
	_target.draw(m_selectedBackground);
	_target.draw(m_portait);
	_target.draw(m_selectedForeground);
}

void FFTACharacterInfos::DrawInfosDispatch(sf::RenderTarget & _target)
{
	_target.draw(m_placeBackground);
	_target.draw(m_portait);
	_target.draw(m_placeForeground);

	_target.draw(m_placePedestalBackground);
	_target.draw(m_placePedestalCharacter);
}

void FFTACharacterInfos::SetCharacter(FFTACharacter * _character)
{
	m_character = _character;

	m_portait = m_character->GetPortait();
	m_portait.setPosition(m_selectedPosition.x, m_selectedPosition.y - m_portait.getGlobalBounds().height);

	m_placePedestalCharacter = m_character->GetAnimation(FFTACharacter::State::Walking, FFTACharacter::Direction::BL);
	m_placePedestalCharacter.setPosition(m_placePedestalPosition.x - 32.f, m_placePedestalPosition.y - 16.f);
}