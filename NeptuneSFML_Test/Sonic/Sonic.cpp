#include "Sonic.h"

#include <NeptuneSFML\EngineCore.h>


void Sonic::Init(const sf::String& _textureName, const sf::Vector2f& _position)
{
	m_position = _position;
	m_currentState = State::Running;
	
	nep::Animation animation;
	std::vector<sf::IntRect> rects;

	rects = { { 6, 7, 29, 40 } };
	animation.Init(nep::TextureMng.GetTexture(_textureName), rects, 1.f);
	animation.setPosition(m_position);
	animation.setOrigin(0.f, 20.f);
	m_animations.push_back(animation);

	rects = { { 8, 49, 24, 40 },{ 36, 49, 36, 40 },{ 75, 49, 29, 40 },{ 110, 49, 26, 40 },{ 141, 49, 37, 40 },{ 183, 49, 32, 40 } };
	//rects = { { 32, 49, -24, 40 }, { 72, 49, -36, 40 }, { 104, 49, -29, 40 }, { 136, 49, -26, 40 }, { 178, 49, -37, 40 }, { 215, 49, -32, 40 } };
	animation.Init(nep::TextureMng.GetTexture(_textureName), rects, 0.15f);
	animation.setPosition(m_position);
	animation.setOrigin(10.f, 20.f);
	m_animations.push_back(animation);
}

void Sonic::Update(float _deltaTime)
{
	m_animations[m_currentState].Update(_deltaTime);
}

void Sonic::Draw(sf::RenderTarget& _target)
{
	_target.draw(m_animations[m_currentState]);
}