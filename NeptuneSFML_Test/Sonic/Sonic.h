#ifndef SONIC_H
#define SONIC_H

#include <NeptuneSFML\Animation\Animation.h>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\System\String.hpp>

class Sonic
{
public:
	Sonic() {}

	void Init(const sf::String& _textureName, const sf::Vector2f& _position);

	void Update(float _deltaTime);
	void Draw(sf::RenderTarget& _target);

private:
	enum State {Idle = 0, Running};

	std::vector<nep::Animation> m_animations;
	sf::Vector2f m_position;
	State m_currentState;
};

#endif // SONIC_H