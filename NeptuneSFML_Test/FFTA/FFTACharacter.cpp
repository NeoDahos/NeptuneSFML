#include "FFTACharacter.h"
#include "Battle\FFTAMap.h"

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\json.hpp>

#include <fstream>
#include <exception>

void FFTACharacter::Init(FFTAMap * _map, const sf::String & _jsonPath)
{
	m_currentState = State::Walking;
	m_currentDirection = Direction::BL;
	m_map = _map;
	m_drawPriority = 0;
	m_isBeingDispatched = false;
	m_isDispatched = false;

	std::ifstream fileStream(_jsonPath.toAnsiString(), std::ifstream::in);
	nlohmann::json fileJson;

	if (fileStream.is_open())
	{
		try
		{
			fileStream >> fileJson;

			m_texture = &nep::TextureMng.GetTexture(fileJson["textureName"]);
			nlohmann::json animationJson;
			nlohmann::json framesTimesJson;
			nlohmann::json frameRectJson;
			std::vector<sf::IntRect> framesRects;
			std::vector<float> framesTimes;
			sf::IntRect frameRect;
			sf::Vector2f origin;
			size_t statesCount;
			size_t framesCount;

			m_portait.setTexture(*m_texture);
			m_portait.setTextureRect({ fileJson["portaitRect"]["x"], fileJson["portaitRect"]["y"], fileJson["portaitRect"]["width"], fileJson["portaitRect"]["height"] });

			statesCount = fileJson["animations"].size();
			for (size_t state = 0; state < statesCount; state++)
			{
				animationJson = fileJson["animations"][state];

				origin.x = animationJson["origin"]["x"];
				origin.y = animationJson["origin"]["y"];


				framesTimesJson = animationJson["framesTimes"];
				framesCount = framesTimesJson.size();
				framesTimes.clear();
				for (size_t frame = 0; frame < framesCount; frame++)
					framesTimes.push_back(framesTimesJson[frame]);

				for (size_t dir = 0; dir < Direction::DirectionCount; dir++)
				{
					framesRects.clear();
					for (size_t frame = 0; frame < framesCount; frame++)
					{
						frameRectJson = animationJson["framesRects"][dir][frame];
						frameRect = { frameRectJson["x"], frameRectJson["y"], frameRectJson["width"], frameRectJson["height"] };
						framesRects.push_back(frameRect);
					}

					AddAnimation(static_cast<State>(state), static_cast<Direction>(dir), *m_texture, framesRects, framesTimes, origin);
				}
			}
		}
		catch (std::exception e)
		{
			printf("An error occured while reading \"%s\".\nMessage : %s.\n\n", _jsonPath.toAnsiString().c_str(), e.what());
			fileStream.close();
		}
	}
}

void FFTACharacter::Update(float _deltaTime)
{
	if (m_isBeingDispatched || m_isDispatched)
		m_animations[(m_currentState * Direction::DirectionCount) + m_currentDirection].Update(_deltaTime);
}

void FFTACharacter::Draw(sf::RenderTarget & _target)
{
	if(m_isBeingDispatched || m_isDispatched)
		_target.draw(m_animations[(m_currentState * Direction::DirectionCount) + m_currentDirection]);
}

void FFTACharacter::AddAnimation(FFTACharacter::State _state, FFTACharacter::Direction _direction, const sf::Texture & _texture, const std::vector<sf::IntRect>& _framesRects, float _framesTime, sf::Vector2f _origin)
{
	m_animations[(_state * Direction::DirectionCount) + _direction].Init(_texture, _framesRects, _framesTime);
	m_animations[(_state * Direction::DirectionCount) + _direction].setOrigin(_origin);
}

void FFTACharacter::AddAnimation(FFTACharacter::State _state, FFTACharacter::Direction _direction, const sf::Texture & _texture, const std::vector<sf::IntRect>& _framesRects, std::vector<float> _framesTimes, sf::Vector2f _origin)
{
	m_animations[(_state * Direction::DirectionCount) + _direction].Init(_texture, _framesRects, _framesTimes);
	m_animations[(_state * Direction::DirectionCount) + _direction].setOrigin(_origin);
}

void FFTACharacter::SetPosition(int _x, int _y)
{
	m_tilePosition.x = _x;
	m_tilePosition.y = _y;

	for (int i = 0; i < (State::StateCount * Direction::DirectionCount); i++)
		m_animations[i].setPosition(m_map->GetIsometricPosition(m_tilePosition));

	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

void FFTACharacter::SetPosition(const sf::Vector2i & _position)
{
	m_tilePosition = _position;

	for (int i = 0; i < (State::StateCount * Direction::DirectionCount); i++)
		m_animations[i].setPosition(m_map->GetIsometricPosition(_position));

	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

void FFTACharacter::Move(int _x, int _y)
{
	m_tilePosition.x += _x;
	m_tilePosition.y += _y;

	for (int i = 0; i < (State::StateCount * Direction::DirectionCount); i++)
		m_animations[i].setPosition(m_map->GetIsometricPosition(m_tilePosition));

	m_drawPriority = m_map->GetTilePriority(m_tilePosition);
}

void FFTACharacter::SetDirection(FFTACharacter::Direction _direction)
{
	m_currentDirection = _direction;
}

int FFTACharacter::GetDrawPriority() const
{
	return m_drawPriority;;
}

sf::Vector2i FFTACharacter::GetTilePosition() const
{
	return m_tilePosition;
}

const sf::Sprite & FFTACharacter::GetPortait() const
{
	return m_portait;
}

const nep::Animation & FFTACharacter::GetAnimation(State _state, Direction _direction) const
{
	return m_animations[(_state * Direction::DirectionCount) + _direction];
}
