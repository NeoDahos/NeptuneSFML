#include "AsciiIntensity.h"

void AsciiIntensity::Compute(const sf::Image& _sourceImage, int _zonePosX, int _zonePosY, int _zoneSizeX, int _zoneSizeY)
{
	Reset();

	const int left = _zoneSizeX / 4;
	const int right = _zoneSizeX - left;
	const int top = _zoneSizeY / 4;
	const int bottom = _zoneSizeY - top;
	int intensity;

	for (int y = 0; y <_zoneSizeY; y++)
	{
		for (int x = 0; x < _zoneSizeX; x++)
		{
			intensity = _sourceImage.getPixel(_zonePosX + x, _zonePosY + y).r;

			if (x <= left)
				m_left += intensity;
			if (x >= right)
				m_right += intensity;
			if (y <= top)
				m_top += intensity;
			if (y >= bottom)
				m_bottom += intensity;

			if (x >= left && x <= right && y >= top && y <= bottom)
				m_center += intensity;
		}
	}

	// Normalize by the area size
	/*const int zoneSize = _zoneSizeX * _zoneSizeY;

	m_left = (m_left * 256) / zoneSize; // Why * 256 ?
	m_right = (m_right * 256) / zoneSize;
	m_top = (m_top * 256) / zoneSize;
	m_bottom = (m_bottom * 256) / zoneSize;
	m_center = (m_center * 256) / zoneSize;*/

	/*m_left /= zoneSize;
	m_right /= zoneSize;
	m_top /= zoneSize;
	m_bottom /= zoneSize;
	m_center /= zoneSize;*/

	// Normalize with a factor of 1000 ?
	const float factor = 1000.f / (_zoneSizeX * _zoneSizeY * 255);
	m_left *= factor;
	m_right *= factor;
	m_top *= factor;
	m_bottom *= factor;
	m_center *= factor;
}

void AsciiIntensity::Reset()
{
	m_left = 0;
	m_right = 0;
	m_top = 0;
	m_bottom = 0;
	m_center = 0;
}
