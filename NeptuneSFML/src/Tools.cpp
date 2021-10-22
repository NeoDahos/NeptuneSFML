#include <NeptuneSFML\Tools.h>

#include <windows.h>
#include <math.h>

namespace nep
{
	std::default_random_engine m_rand_generator;
	std::uniform_real_distribution<float> m_rand_realDistribution(0.f, 1.f);

	sf::Color LerpColor(const sf::Color& _min, const sf::Color& _max, float _time)
	{
		return sf::Color(Lerp(_min.r, _max.r, _time), Lerp(_min.g, _max.g, _time), Lerp(_min.b, _max.b, _time), Lerp(_min.a, _max.a, _time));
	}

	float GetRandomValue()
	{
		return m_rand_realDistribution(m_rand_generator);
	}

	int GetRandomValue(int min, int max)
	{
		std::uniform_int_distribution<int> rand_intDistribution(min, max);
		return rand_intDistribution(m_rand_generator);
	}
	std::string OpenFilePicker()
	{
		char sFile[255];
		OPENFILENAME openFileName;
		ZeroMemory(&openFileName, sizeof(openFileName));

		openFileName.lStructSize = sizeof(openFileName);
		openFileName.hwndOwner = NULL;
		openFileName.lpstrFile = sFile;
		openFileName.lpstrFile[0] = '\0';
		openFileName.nMaxFile = sizeof(sFile);
		openFileName.lpstrFileTitle = NULL;
		openFileName.nMaxFileTitle = 0;
		GetOpenFileName(&openFileName);

		return openFileName.lpstrFile;
	}
}