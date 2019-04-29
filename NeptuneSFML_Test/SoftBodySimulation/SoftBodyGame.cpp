#include "SoftBodyGame.h"
#include <NeptuneSFML\Tools.h>

// http://panoramix.ift.uni.wroc.pl/~maq/soft2d/howtosoftbody.pdf
// http://paulbourke.net/miscellaneous/particle/

void SoftBodyGame::Init(sf::RenderWindow & _window)
{
	std::vector<SoftBodyObject::Point> points;
	const sf::Vector2f windowSize(_window.getSize());
	const float ballRadius = 50.f;
	const int pointCount = 20;

	SoftBodyObject::s_bounds = { 0.f, -windowSize.y, windowSize.x, windowSize.y * 2.f };
	SoftBodyObject::s_gravity = 100.f;

	for (int i = 0; i < pointCount; i++)
	{
		points.emplace_back();
		points[i].position.x = ballRadius * sin(i * nep::TWO_PI / pointCount) + windowSize.x / 2.f;
		points[i].position.y = ballRadius * cos(i * nep::TWO_PI / pointCount) + windowSize.y / 2.f;
	}
	/*points.emplace_back();
	points[0].position.x = -ballRadius + windowSize.x / 2.f -10.f;
	points[0].position.y = -ballRadius + windowSize.y / 2.f +10.f;

	points.emplace_back();
	points[1].position.x = ballRadius + windowSize.x / 2.f -10.f;
	points[1].position.y = -ballRadius + windowSize.y / 2.f -10.f;

	points.emplace_back();
	points[2].position.x = ballRadius + windowSize.x / 2.f +10.f;
	points[2].position.y = ballRadius + windowSize.y / 2.f -10.f;

	points.emplace_back();
	points[3].position.x = -ballRadius + windowSize.x / 2.f +10.f;
	points[3].position.y = ballRadius + windowSize.y / 2.f +10.f;*/

	m_ball.Init(points, 1.f, 500000.f, 1500.f, 15.f);
}

void SoftBodyGame::DeInit()
{
}

void SoftBodyGame::HandleEvent(const sf::Event & _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::F)
			SoftBodyObject::s_drawVelocities = !SoftBodyObject::s_drawVelocities;
		else if (_event.key.code == sf::Keyboard::N)
			SoftBodyObject::s_drawNormals = !SoftBodyObject::s_drawNormals;
	}
}

void SoftBodyGame::Update(float _deltaTime)
{
	m_ball.Update(_deltaTime);
}

void SoftBodyGame::Draw(sf::RenderTarget & _target, const sf::RenderStates& _states)
{
	m_ball.Draw(_target);
}