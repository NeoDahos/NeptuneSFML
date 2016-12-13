#include <NeptuneSFML\Animation\Animation.h>

namespace nep
{
	Animation::Animation()
	{
		m_framesRects;
		m_frameCount = 0;
		m_frameIndex = 0;
		m_actualFrameTime = 0.f;
		m_speed = 1.f;
		m_isPlaying = false;
	}

	Animation::~Animation()
	{
	}

	void Animation::Init(const sf::Texture& _texture, int _rowCount, int _columnCount, int _frameCount, float _framesTime)
	{
		m_frameCount = _frameCount;
		m_frameIndex = 0;
		setTexture(_texture);

		sf::IntRect frameRect = getTextureRect();
		frameRect.width /= _columnCount;
		frameRect.height /= _rowCount;

		for (int y = 0; y < _rowCount; y++)
		{
			for (int x = 0; x < _columnCount && (x + y * _columnCount) < m_frameCount; x++)
			{
				m_framesTimes.push_back(_framesTime);
				m_framesRects.push_back(frameRect);
				frameRect.left += frameRect.width;
			}
			frameRect.left = 0;
			frameRect.top += frameRect.height;
		}

		setTextureRect(m_framesRects[m_frameIndex]);
		m_isPlaying = true;
	}

	void Animation::Init(const sf::Texture & _texture, int _rowCount, int _columnCount, int _frameCount, const std::vector<float> & _framesTimes)
	{
		m_frameCount = _frameCount;
		m_frameIndex = 0;
		setTexture(_texture);

		sf::IntRect frameRect = getTextureRect();
		frameRect.width /= _columnCount;
		frameRect.height /= _rowCount;

		for (int y = 0; y < _rowCount; y++)
		{
			for (int x = 0; x < _columnCount && (x + y * _columnCount) < m_frameCount; x++)
			{
				m_framesTimes.push_back(_framesTimes[x + y * _columnCount]);
				m_framesRects.push_back(frameRect);
				frameRect.left += frameRect.width;
			}
			frameRect.left = 0;
			frameRect.top += frameRect.height;
		}

		setTextureRect(m_framesRects[m_frameIndex]);
		m_isPlaying = true;
	}

	void Animation::Init(const sf::Texture & _texture, const std::vector<sf::IntRect>& _framesRects, float _framesTime)
	{
		m_framesRects = _framesRects;
		m_frameCount = m_framesRects.size();

		for (int i = 0; i < m_frameCount; i++)
			m_framesTimes.push_back(_framesTime);

		setTexture(_texture);
		m_frameIndex = 0;
		setTextureRect(m_framesRects[m_frameIndex]);
		m_actualFrameTime = 0.f;
		m_speed = 1.f;
		m_isPlaying = true;
	}

	void Animation::Init(const sf::Texture & _texture, const std::vector<sf::IntRect>& _framesRects, const std::vector<float> & _framesTimes)
	{
		m_framesRects = _framesRects;
		m_frameCount = m_framesRects.size();
		m_framesTimes = _framesTimes;
		setTexture(_texture);
		m_frameIndex = 0;
		setTextureRect(m_framesRects[m_frameIndex]);
		m_actualFrameTime = 0.f;
		m_speed = 1.f;
		m_isPlaying = true;
	}

	void Animation::Update(float _deltaTime)
	{
		if (m_isPlaying && m_frameCount > 1)
		{
			m_actualFrameTime += _deltaTime;

			if (m_actualFrameTime >= m_framesTimes[m_frameIndex] / m_speed)
			{
				//m_actualFrameTime -= (m_framesTimes[m_frameIndex] / m_speed);
				m_actualFrameTime = 0.f;
				m_frameIndex = (m_frameIndex + 1) % m_frameCount;

				setTextureRect(m_framesRects[m_frameIndex]);
			}
		}
	}

	void Animation::Play()
	{
		m_isPlaying = true;
	}

	void Animation::Pause()
	{
		m_isPlaying = false;
	}

	void Animation::Stop()
	{
		m_isPlaying = false;
		m_actualFrameTime = 0.0f;
		m_frameIndex = 0;
	}

	void Animation::SetPlaySpeed(float _speed)
	{
		m_speed = _speed;
	}
}