#ifndef DIALOG_MANAGER_H
#define DIALOG_MANAGER_H
#include <NeptuneSFML\Export.h>

#include <SFML\System\NonCopyable.hpp>

#include <NeptuneSFML\Dialog\Dialog.h>

#include <map>

namespace nep
{
	class NEPTUNE_API DialogManager : public sf::NonCopyable
	{
	public:
		void LoadDialogs(const std::string& _filePath);

	private:
		void ParseDialog(std::string& _buffer);

		std::map<std::string, Dialog> m_dialogs;
	};
}

#endif // DIALOG_MANAGER_H