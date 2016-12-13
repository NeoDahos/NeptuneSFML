#include <NeptuneSFML\Dialog\Dialog.h>

namespace nep
{
	Dialog::Dialog(const std::string& _characterName, const std::string& _text) : m_characterName(_characterName), m_text(_text)
	{
	}

	const std::string& Dialog::GetCharacterName() const
	{
		return m_characterName;
	}

	const std::string& Dialog::GetText() const
	{
		return m_text;
	}
}