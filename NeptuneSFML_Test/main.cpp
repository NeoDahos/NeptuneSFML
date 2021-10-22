#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\Text.hpp>

#include <NeptuneSFML\EngineCore.h>
#include <NeptuneSFML\Tools.h>

//#include "Shmup\ShmupGame.h"
//#include "CreeperWorld\CreeperWorldGame.h"
//#include "Particles\ParticlesGame.h"
//#include "PathFinding\PathFindingGame.h"
//#include "FFTA\FFTAGame.h"
//#include "SoftBodySimulation\SoftBodyGame.h"
//#include "GameOfLife\GameOfLifeGame.h"
//#include "Noita\NoitaGame.h"
#include "AsciiArtConverter\AsciiArtConverterGame.h"

void main()
{
	sf::RenderWindow window;
	sf::Event event;
	sf::Clock timer;
	sf::Text fpsText;
	float deltaTime;

	//window.create(sf::VideoMode(1920, 1080), "NeptuneSFML Test", sf::Style::Fullscreen);
	window.create(sf::VideoMode(1280, 720), "NeptuneSFML Test");
	//window.setFramerateLimit(60);

	fpsText.setFont(nep::FontMng.GetFont("default"));
	fpsText.setPosition(550.f, 0.f);
	fpsText.setCharacterSize(16);

	AsciiArtConverterGame game;
	game.Init(window);

	timer.restart();
	while (window.isOpen())
	{
		deltaTime = timer.restart().asSeconds();
		//fpsText.setString(nep::String_format("%.0f FPS (%.4f ms)", (1.f / deltaTime), (deltaTime * 1000.f)));

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

		//window.draw(fpsText);
		window.display();
	}

	game.DeInit();
}