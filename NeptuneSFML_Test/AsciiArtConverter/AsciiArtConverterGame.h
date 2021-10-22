#pragma once

#include <NeptuneSFML/Game.h>
#include <SFML/Graphics/Text.hpp>

#include "AsciiIntensity.h"

// https://stackoverflow.com/questions/32987103/image-to-ascii-art-conversion
// https://yuukidach.github.io/2020/02/19/practice/image-to-asciiart/

class AsciiArtConverterGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target, const sf::RenderStates& _states = sf::RenderStates::Default);

	void ComputeStringFromImage(const sf::Image& _imageToConvert);
	void ComputeStringFromImageV2(const sf::Image& _imageToConvert);
	void ComputeStringFromImageV3(const sf::Image& _imageToConvert);

	sf::Image ConvertToGrayScale(const sf::Image& _sourceImage) const;

private:
	void ComputeCharIntensities();
	void SelectFileToOpen();

	std::vector<AsciiIntensity> m_charIntensities;
	sf::Image m_imageToConvert;
	sf::Text m_textResult;
	sf::Vector2f m_textScale;
	sf::Vector2u m_textCharSize;
	sf::Vector2u m_imageZoneSize;
};