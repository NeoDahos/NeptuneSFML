#ifndef ANIMATION_H
#define ANIMATION_H
#include <NeptuneSFML\Export.h>

#include <SFML\Graphics\Sprite.hpp>

#include <vector>

namespace nep
{
	class NEPTUNE_API Animation : public sf::Sprite
	{
	public:
		Animation();
		~Animation();
		
		void Init(const sf::Texture& _texture, int _rowCount, int _columnCount, int _frameCount, float _framesTime);
		void Init(const sf::Texture& _texture, int _rowCount, int _columnCount, int _frameCount, const std::vector<float> & _framesTimes);
		void Init(const sf::Texture& _texture, const std::vector<sf::IntRect>& _framesRects, float _framesTime);
		void Init(const sf::Texture& _texture, const std::vector<sf::IntRect>& _framesRects, const std::vector<float> & _framesTimes);
		void Update(float _deltaTime);

		void Play();
		void Pause();
		void Stop();

		void SetPlaySpeed(float _speed);

	private:
		std::vector<sf::IntRect> m_framesRects;
		std::vector<float> m_framesTimes;

		float m_speed = 1.0f;
		float m_actualFrameTime = 0.0f;

		int m_frameCount = 1;
		int m_frameIndex = 0;

		bool m_isPlaying = true;
	};
}

#endif // ANIMATION_H