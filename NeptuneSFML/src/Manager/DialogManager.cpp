#include <NeptuneSFML\Manager\DialogManager.h>

#include <fstream>

namespace nep
{
	void DialogManager::LoadDialogs(const std::string& _filePath)
	{
		std::ifstream dialogFile(_filePath);

		if (dialogFile.is_open())
		{
			std::string buffer;
			std::string line;

			while (std::getline(dialogFile, line))
			{
				buffer += line;
				if (line.back() == ']')
					ParseDialog(buffer);
				else
					buffer += '\n';
			}

			dialogFile.close();
		}
		else
			printf("Unable to open file : %s\n", _filePath.c_str());
	}

	void DialogManager::ParseDialog(std::string& _buffer)
	{
		std::string key;
		std::string characterName;
		std::string text;
		
		size_t findPosBegin = 0;
		size_t findPosEnd = 0;

		while (_buffer.length() != 0)
		{
			findPosBegin = _buffer.find_first_of("[", findPosBegin);
			findPosEnd = _buffer.find_first_of("]", findPosBegin);

			if (key.empty())
				key = _buffer.substr(findPosBegin + 1, findPosEnd - findPosBegin - 1);
			else if (characterName.empty())
				characterName = _buffer.substr(findPosBegin + 1, findPosEnd - findPosBegin - 1);
			else
			{
				text = _buffer.substr(findPosBegin + 1, findPosEnd - findPosBegin - 1);
				m_dialogs.emplace(key, Dialog(characterName, text));
			}

			_buffer = _buffer.substr(findPosEnd + 1);
		}
	}

}