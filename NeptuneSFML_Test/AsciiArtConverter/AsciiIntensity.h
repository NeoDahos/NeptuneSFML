#pragma once

#include <SFML/Graphics/Image.hpp>

class AsciiIntensity
{
public:
	void Compute(const sf::Image& _sourceImage, int _zonePosX, int _zonPosY, int _zoneSizeX, int _zonSizeY);
	void Reset();

	int m_left { 0 };
	int m_right { 0 };
	int m_top { 0 };
	int m_bottom { 0 };
	int m_center { 0 };
	char m_char { 0 };
};