#ifndef GAME_H
#define GAME_H
#include <SFML\System\NonCopyable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

namespace nep
{
	class Game : public sf::NonCopyable
	{
	public:
		virtual void Init(sf::RenderWindow& _window) = 0;
		virtual void DeInit() = 0;
		virtual void HandleEvent(const sf::Event & _event) = 0;
		virtual void Update(float _deltaTime) = 0;
		virtual void Draw(sf::RenderTarget& _target) = 0;
	};
}

#endif // GAME_H