#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\NonCopyable.hpp>
#include <SFML\Graphics\Text.hpp>

namespace nep
{
	class Dialog;

	class NEPTUNE_API DialogBox : public sf::NonCopyable
	{
	public:
		DialogBox(const sf::Font& _font);

		void Draw(sf::RenderTarget& _renderTarget);

	private:
		sf::Sprite m_background;
		sf::Text m_characterName;
		sf::Text m_texte;
		Dialog* m_dialog;
	};
}

#endif // DIALOG_BOX_H