#include <NeptuneSFML\Manager\TextureManager.h>

namespace nep
{
	TextureManager::TextureManager()
	{
	}

	TextureManager::~TextureManager()
	{
	}

	bool TextureManager::AddTexture(const sf::String& _name, const sf::String& _path)
	{
		sf::Texture* texture = new sf::Texture;
		if (texture->loadFromFile(_path))
		{
			m_texturesMap.emplace(_name, texture);
			return true;
		}

		return false;
	}

	sf::Texture& TextureManager::GetTexture(const sf::String& _name)
	{
		return *m_texturesMap[_name];
	}

	bool TextureManager::EraseTexture(const sf::String& _name)
	{
		if (m_texturesMap.find(_name) != m_texturesMap.end())
		{
			delete m_texturesMap[_name];
			m_texturesMap.erase(_name);

			return true;
		}

		return false;
	}

	void TextureManager::ClearAllTextures()
	{
		for (auto&& texture : m_texturesMap)
		{
			if (texture.second)
				delete texture.second;
		}

		m_texturesMap.clear();
	}
}