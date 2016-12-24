#include <NeptuneSFML\Manager\FontManager.h>

namespace nep
{
	bool FontManager::AddFont(const sf::String& _name, const sf::String& _path)
	{
		sf::Font* font = new sf::Font;
		if (font->loadFromFile(_path))
		{
			m_fontsMap.emplace(_name, font);
			return true;
		}

		return false;
	}

	sf::Font& FontManager::GetFont(const sf::String& _name)
	{
		return *m_fontsMap[_name];
	}

	bool FontManager::EraseFont(const sf::String& _name)
	{
		if (m_fontsMap.find(_name) != m_fontsMap.end())
		{
			delete m_fontsMap[_name];
			m_fontsMap.erase(_name);

			return true;
		}

		return false;
	}

	void FontManager::ClearAllFonts()
	{
		for (auto&& font : m_fontsMap)
		{
			if (font.second)
				delete font.second;
		}

		m_fontsMap.clear();
	}
}