#include <NeptuneSFML\Dialog\DialogBox.h>
#include <NeptuneSFML\Dialog\Dialog.h>

#include <SFML\Graphics\RenderTarget.hpp>

namespace nep
{
	DialogBox::DialogBox(const sf::Font& _font)
	{
		m_characterName.setFont(_font);
		m_texte.setFont(_font);
	}

	void DialogBox::Draw(sf::RenderTarget& _renderTarget)
	{
		_renderTarget.draw(m_background);
		_renderTarget.draw(m_characterName);
		_renderTarget.draw(m_texte);
	}
}