#include "AsciiArtConverterGame.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <NeptuneSFML/EngineCore.h>
#include <NeptuneSFML/Tools.h>

void AsciiArtConverterGame::Init(sf::RenderWindow& _window)
{
	nep::FontMng.AddFont("AsciiArt", "../Data/Font/DejaVuSansMono.ttf");

	m_textCharSize.y = 7;

	m_textResult.setFont(nep::FontMng.GetFont("AsciiArt"));
	m_textResult.setCharacterSize(m_textCharSize.y);
	m_textScale = sf::Vector2f(1.f, 1.f);

	SelectFileToOpen();
}

void AsciiArtConverterGame::DeInit()
{
	nep::FontMng.EraseFont("AsciiArt");
}

void AsciiArtConverterGame::HandleEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::O)
		{
			SelectFileToOpen();
		}
		else if (_event.key.code == sf::Keyboard::Subtract)
		{
			m_textCharSize.y--;
			if (m_textCharSize.y < 1)
				m_textCharSize.y = 1;

			m_textResult.setCharacterSize(m_textCharSize.y);

			ComputeCharIntensities();
			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::Add)
		{
			m_textCharSize.y++;
			m_textResult.setCharacterSize(m_textCharSize.y);

			ComputeCharIntensities();
			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::Left)
		{
			m_imageZoneSize.x--;
			if (m_imageZoneSize.x < 1)
				m_imageZoneSize.x = 1;

			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::Down)
		{
			m_imageZoneSize.y--;
			if (m_imageZoneSize.y < 1)
				m_imageZoneSize.y = 1;

			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::Right)
		{
			m_imageZoneSize.x++;

			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::Up)
		{
			m_imageZoneSize.y++;

			ComputeStringFromImageV3(m_imageToConvert);
		}
		else if (_event.key.code == sf::Keyboard::PageDown)
		{
			m_textScale *= 0.9f;
			m_textResult.setScale(m_textScale);
		}
		else if (_event.key.code == sf::Keyboard::PageUp)
		{
			m_textScale *= 1.1f;
			m_textResult.setScale(m_textScale);
		}
	}
}

void AsciiArtConverterGame::Update(float _deltaTime)
{
}

void AsciiArtConverterGame::Draw(sf::RenderTarget& _target, const sf::RenderStates& _states)
{
	_target.draw(m_textResult);
}

void AsciiArtConverterGame::ComputeStringFromImage(const sf::Image& _imageToConvert)
{
	//const char charList[] = " .,:;ox%#@"; // Usable characters ordered by intensity
	const char charList[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. "; // Usable characters ordered by intensity
	const int charListLength = sizeof(charList) / sizeof(charList[0]) -1;

	std::string resultString;
	sf::Vector2u imageSize = _imageToConvert.getSize();

	for (unsigned int y = 0; y < imageSize.y; y++)
	{
		for (unsigned int x = 0; x < imageSize.x; x++)
		{
			sf::Color pixelColor = _imageToConvert.getPixel(x, y);
			//resultString += charList[(int)((pixelColor.r / 255.f) * charListLength)];
			resultString += charList[charListLength - (int)(pixelColor.r / 255.f * charListLength)];
		}

		resultString += "\n";
	}

	m_textResult.setString(resultString);
}

void AsciiArtConverterGame::ComputeStringFromImageV2(const sf::Image& _imageToConvert)
{
	m_imageZoneSize = m_textCharSize;
	ComputeStringFromImageV3(_imageToConvert);
}

void AsciiArtConverterGame::ComputeStringFromImageV3(const sf::Image& _imageToConvert)
{
	AsciiIntensity currentIntensity;
	std::string resultString;
	sf::Vector2u imageSize = _imageToConvert.getSize();

	imageSize.x -= imageSize.x % m_imageZoneSize.x;
	imageSize.y -= imageSize.y % m_imageZoneSize.y;

	for (unsigned int y = 0; y < imageSize.y; y += m_imageZoneSize.y)
	{
		for (unsigned int x = 0; x < imageSize.x; x += m_imageZoneSize.x)
		{
			// Compute intensity
			currentIntensity.Compute(_imageToConvert, x, y, m_imageZoneSize.x, m_imageZoneSize.y);

			// Find the closest match in the available characters
			int maxDistance = -1;

			AsciiIntensity& closestCharIntensity = m_charIntensities[0];
			for (const auto& charIntensity : m_charIntensities)
			{
				int distance = abs(charIntensity.m_left - currentIntensity.m_left) +
					abs(charIntensity.m_right - currentIntensity.m_right) +
					abs(charIntensity.m_top - currentIntensity.m_top) +
					abs(charIntensity.m_bottom - currentIntensity.m_bottom) +
					abs(charIntensity.m_center - currentIntensity.m_center);

				if ((maxDistance < 0) || (maxDistance > distance))
				{
					maxDistance = distance;
					closestCharIntensity = charIntensity;
				}
			}

			resultString += closestCharIntensity.m_char;
		}

		resultString += "\n";
	}

	m_textResult.setString(resultString);
}

sf::Image AsciiArtConverterGame::ConvertToGrayScale(const sf::Image& _sourceImage) const
{
	sf::Image grayScaleImag(_sourceImage);	
	sf::Vector2u imageSize = grayScaleImag.getSize();

	for (unsigned int y = 0; y < imageSize.y; y++)
	{
		for (unsigned int x = 0; x < imageSize.x; x++)
		{
			sf::Color pixelColor = grayScaleImag.getPixel(x, y);
			int grayValue = (int)(0.299f * pixelColor.r + 0.587f * pixelColor.g + 0.114f * pixelColor.b);

			grayScaleImag.setPixel(x, y, sf::Color(grayValue, grayValue, grayValue));
		}
	}

	return grayScaleImag;
}

void AsciiArtConverterGame::ComputeCharIntensities()
{
	sf::Text charText((char)32, nep::FontMng.GetFont("AsciiArt"), m_textCharSize.y);
	sf::FloatRect charBounds = charText.getLocalBounds();
	m_textCharSize = sf::Vector2u((unsigned int)charBounds.width, charText.getCharacterSize());
	sf::RenderTexture charRenderTexture;
	charRenderTexture.create(m_textCharSize.x, m_textCharSize.y);

	m_charIntensities.clear();

	//for (int i = 0, c = 32; c < 128; i++, c++)
	for (int i = 0, c = 32; c < 255; i++, c++)
	{
		charText.setString((char)c);
		charRenderTexture.draw(charText);
		charRenderTexture.display();
		sf::Image charImage = charRenderTexture.getTexture().copyToImage();

		AsciiIntensity currentIntensity;
		currentIntensity.Compute(charImage, 0, 0, m_textCharSize.x, m_textCharSize.y);
		currentIntensity.m_char = (char)c;
		m_charIntensities.push_back(currentIntensity);
	}
}

void AsciiArtConverterGame::SelectFileToOpen()
{
	std::string filePath = nep::OpenFilePicker();

	if (!m_imageToConvert.loadFromFile(filePath))
	{
		std::cout << "Error while loading the image." << std::endl;
		return;
	}

	m_imageToConvert = ConvertToGrayScale(m_imageToConvert);

	ComputeCharIntensities();

	m_imageZoneSize = m_textCharSize;

	ComputeStringFromImageV3(m_imageToConvert);
}
