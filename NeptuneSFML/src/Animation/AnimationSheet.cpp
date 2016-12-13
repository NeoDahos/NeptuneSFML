#include <NeptuneSFML\Animation\AnimationSheet.h>

namespace nep
{
	AnimationSheet::AnimationSheet()
	{
	}

	AnimationSheet::~AnimationSheet()
	{
	}

	void AnimationSheet::SetPlaySpeed(float _speed)
	{
		m_speed = _speed;
	}

	void AnimationSheet::AddAnimation(const AnimationInfos& _animation)
	{
		m_animations.push_back(_animation);
	}

	void AnimationSheet::Init(const sf::Texture& _texture, bool _useCommonTimer)
	{
		m_useCommonTimer = _useCommonTimer;
		setTexture(_texture);
		setTextureRect(m_frameRect);
	}

	void AnimationSheet::Update(float _deltaTime)
	{
		if (m_isPlaying)
		{
			AnimationInfos& anim = m_animations[m_animationIndex];

			if (m_useCommonTimer)
			{
				m_globalFrameTime += _deltaTime;

				if (m_globalFrameTime >= anim.frameDuration / m_speed)
				{
					m_globalFrameIndex = (m_globalFrameIndex + 1) % anim.frameCount;
					m_globalFrameTime -= (anim.frameDuration / m_speed);
					m_frameRect.left = anim.animRect.left + anim.frameSize.x * (m_globalFrameIndex % anim.columnCount);
					m_frameRect.top = anim.animRect.top + anim.frameSize.y * (m_globalFrameIndex / anim.columnCount);
					setTextureRect(m_frameRect);
				}
			}
			else
			{
				anim.actualFrameTime += _deltaTime;

				if (anim.actualFrameTime >= anim.frameDuration / m_speed)
				{
					anim.frameIndex = (anim.frameIndex + 1) % anim.frameCount;
					anim.actualFrameTime -= (anim.frameDuration / m_speed);
					m_frameRect.left = anim.animRect.left + anim.frameSize.x * (anim.frameIndex % anim.columnCount);
					m_frameRect.top = anim.animRect.top + anim.frameSize.y * (anim.frameIndex / anim.columnCount);
					setTextureRect(m_frameRect);
				}
			}
		}
	}

	void AnimationSheet::Play()
	{
		if (m_animations.size() > 0)
			m_isPlaying = true;
	}

	void AnimationSheet::Play(unsigned short _animationIndex)
	{
		if (_animationIndex < m_animations.size())
		{
			m_animationIndex = _animationIndex;
			AnimationInfos& anim = m_animations[m_animationIndex];

			if (m_useCommonTimer)
			{
				m_frameRect.left = anim.animRect.left + anim.frameSize.x * (m_globalFrameIndex % anim.columnCount);
				m_frameRect.top = anim.animRect.top + anim.frameSize.y * (m_globalFrameIndex / anim.columnCount);
			}
			else
			{
				m_frameRect.left = anim.animRect.left + anim.frameSize.x * (anim.frameIndex % anim.columnCount);
				m_frameRect.top = anim.animRect.top + anim.frameSize.y * (anim.frameIndex / anim.columnCount);
			}

			m_frameRect.width = m_animations[m_animationIndex].frameSize.x;
			m_frameRect.height = m_animations[m_animationIndex].frameSize.y;
			setTextureRect(m_frameRect);
			m_isPlaying = true;
		}
	}

	void AnimationSheet::Pause()
	{
		m_isPlaying = false;
	}

	void AnimationSheet::Stop()
	{
		m_isPlaying = false;
		m_globalFrameTime = 0.0f;
		m_globalFrameIndex = 0;
		m_animations[m_animationIndex].actualFrameTime = 0.0f;
		m_animations[m_animationIndex].frameIndex = 0;
	}
}