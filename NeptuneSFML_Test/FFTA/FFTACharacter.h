#ifndef FFTA_CHARACTER_H
#define FFTA_CHARACTER_H

#include <NeptuneSFML\Animation\Animation.h>

#include <SFML\System\String.hpp>

class FFTAMap;

class FFTACharacter
{
public:
	enum State {Walking = 0, StateCount};
	enum Direction {BL = 0, TL, TR, BR, DirectionCount};

	void Init(FFTAMap * _map, const sf::String & _jsonPath);
	void Update(float _deltaTime);
	void Draw(sf::RenderTarget & _target);

	void AddAnimation(FFTACharacter::State _state, FFTACharacter::Direction _direction, const sf::Texture & _texture, const std::vector<sf::IntRect> & _framesRects, float _framesTime, sf::Vector2f _origin = sf::Vector2f());
	void AddAnimation(FFTACharacter::State _state, FFTACharacter::Direction _direction, const sf::Texture & _texture, const std::vector<sf::IntRect> & _framesRects, std::vector<float> _framesTimes, sf::Vector2f _origin = sf::Vector2f());

	void SetPosition(int _x, int _y);
	void SetPosition(const sf::Vector2i & _position);
	void Move(int _x, int _y);
	void SetDirection(FFTACharacter::Direction _direction);

	int GetDrawPriority() const;
	sf::Vector2i GetTilePosition() const;
	const sf::Sprite & GetPortait() const;
	const nep::Animation & GetAnimation(State _state, Direction _direction) const;

	bool m_isBeingDispatched;
	bool m_isDispatched;

private:
	nep::Animation m_animations[State::StateCount * Direction::DirectionCount];
	sf::Sprite m_portait;
	sf::Vector2i m_tilePosition;
	State m_currentState;
	Direction m_currentDirection;
	FFTAMap * m_map;
	sf::Texture * m_texture;
	int m_drawPriority;
};

#endif // FFTA_CHARACTER_H