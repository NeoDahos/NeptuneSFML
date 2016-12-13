#ifndef PARTICLES_GAME_H
#define PARTICLES_GAME_H
#include <NeptuneSFML\Game.h>

#include <NeptuneSFML\Particles\Systems\VertexParticleSystem.h>
#include <NeptuneSFML\Particles\Effectors\Repeller.h>

class ParticlesGame : public nep::Game
{
public:
	virtual void Init(sf::RenderWindow& _window);
	virtual void DeInit();
	virtual void HandleEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderTarget& _target);

private:
	std::vector<nep::Repeller> m_repellers;
	nep::VertexParticleSystem m_vertexParticleSystem;
	sf::Vector2f m_gravity;
};

#endif //PARTICLES_GAME_H