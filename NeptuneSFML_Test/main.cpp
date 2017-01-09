#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\Text.hpp>

#include <NeptuneSFML\EngineCore.h>

//#include "Zelda\ZeldaGame.h"
//#include "Shmup\ShmupGame.h"
//#include "Isaac\IsaacGame.h"
//#include "CreeperWorld\CreeperWorldGame.h"
//#include "Sonic\SonicGame.h"
#include "Particles\ParticlesGame.h"
//#include "PathFinding\PathFindingGame.h"
//#include "FFTA\FFTAGame.h"
//#include "SoftBodySimulation\SoftBodyGame.h"

void main()
{
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock timer;
	sf::Text fpsText;
	float deltaTime;
	
	window.create(sf::VideoMode(800, 600), "NeptuneSFML Test");
	window.setFramerateLimit(60);
	
	fpsText.setFont(nep::FontMng.GetFont("default"));
	fpsText.setFillColor(sf::Color::White);
	fpsText.setPosition(550.f, 0.f);
	
	ParticlesGame game;
	game.Init(window);

	timer.restart();
	while (window.isOpen())
	{
		deltaTime = timer.restart().asSeconds();
		fpsText.setString("FPS : " + std::to_string(1.f / deltaTime));

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}

			game.HandleEvent(event);
		}
		
		game.Update(deltaTime);

		window.clear(sf::Color::Black);
		
		game.Draw(window);

		window.draw(fpsText);
		window.display();
	}

	game.DeInit();
}