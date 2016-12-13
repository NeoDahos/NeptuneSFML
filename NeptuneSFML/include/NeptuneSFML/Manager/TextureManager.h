#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Texture.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\NonCopyable.hpp>

#include <map>

namespace nep
{
	class NEPTUNE_API TextureManager : public sf::NonCopyable
	{
	public:
		TextureManager();
		~TextureManager();

		bool AddTexture(const sf::String& _name, const sf::String& _path);
		sf::Texture& GetTexture(const sf::String& _name);
		bool EraseTexture(const sf::String& _name);
		void ClearAllTextures();

	private:
		std::map<sf::String, sf::Texture*> m_texturesMap;
	};
}

#endif // TEXTURE_MANAGER_H