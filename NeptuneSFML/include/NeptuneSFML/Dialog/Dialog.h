#ifndef DIALOG_H
#define DIALOG_H
#include <NeptuneSFML\Export.h>

#include <string>

namespace nep
{
	class NEPTUNE_API Dialog
	{
	public:
		Dialog(const std::string& _characterName, const std::string& _text);

		const std::string& GetCharacterName() const;
		const std::string& GetText() const;

	private:
		std::string m_characterName;
		std::string m_text;
	};
}

#endif // DIALOG_H