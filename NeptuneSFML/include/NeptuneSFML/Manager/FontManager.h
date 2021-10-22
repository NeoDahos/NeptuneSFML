#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Font.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\NonCopyable.hpp>

#include <map>

namespace nep
{
	class NEPTUNE_API FontManager : public sf::NonCopyable
	{
	public:
		~FontManager();

		bool AddFont(const sf::String& _name, const sf::String& _path);
		sf::Font& GetFont(const sf::String& _name);
		bool EraseFont(const sf::String& _name);
		void ClearAllFonts();

	private:
		std::map<sf::String, sf::Font*> m_fontsMap;
	};
}

#endif // FONT_MANAGER_H