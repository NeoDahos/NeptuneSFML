#pragma once

#include <SFML\Graphics\Text.hpp>

#include <NeptuneSFML\Game.h>

#include "NoitaChunkParticleManager.h"

/* Usefull links
* https://blog.winter.dev/2020/falling-sand-games/
* https://www.youtube.com/watch?v=wZJCQQPaGZI
* https://www.youtube.com/watch?v=VLZjd_Y1gJ8
* 
* Not seen yet
* https://www.youtube.com/watch?v=dogbxKqie_E
* https://www.youtube.com/watch?v=5Ka3tbbT-9E
*/

class NoitaGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(const sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target, const sf::RenderStates& _states = sf::RenderStates::Default);


private:
	void SpawnParticles(const sf::Vector2i& center, NoitaParticle::Type typeToSpawn);
	void SpawnParticlesInSquare(const sf::Vector2i& center, int radius, const NoitaParticle& particleToSpawn);
	void SpawnParticlesInCircle(const sf::Vector2i& center, int radius, const NoitaParticle& particleToSpawn);

	static constexpr const float BaseViewMoveSpeed = 100.f;
	static constexpr const float TimeBetweenUIUpdate = 0.25f;

	NoitaChunkParticleManager m_chunkManager;
	sf::Text m_fpsText;
	sf::Text m_simulationInfosText;
	sf::Text m_particleToSpawnText;
	sf::View m_view;
	sf::Vector2f m_viewMoveDirection;
	float m_currentViewZoom{ 1.f };
	float m_timeUntilNextUIUpdate{ TimeBetweenUIUpdate };
	sf::RenderWindow* m_windowRef{};
	int m_brushSize{ 2 };
	NoitaParticle::Type m_particleTypeToSpawn{ NoitaParticle::Type::Sand };
	bool m_isLeftMouseButtonClicked{};
	bool m_isRightMouseButtonClicked{};
	bool m_isViewMoving{};
};