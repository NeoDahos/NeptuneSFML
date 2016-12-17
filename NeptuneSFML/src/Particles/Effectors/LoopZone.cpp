#include <NeptuneSFML\Particles\Effectors\LoopZone.h>
#include <NeptuneSFML\Particles\VertexParticle.h>

void nep::LoopZone::Init(const sf::FloatRect & _zone, bool _activated)
{
	m_isActive = _activated;
	m_zone = _zone;
}

void nep::LoopZone::Apply(Particle * const _particle)
{
	sf::Vector2f newPosition = _particle->GetPosition();

	if (newPosition.x < m_zone.left)
		newPosition.x += m_zone.width;
	else if (newPosition.x >= m_zone.left + m_zone.width)
		newPosition.x -= m_zone.width;

	if (newPosition.y < m_zone.top)
		newPosition.y += m_zone.height;
	else if (newPosition.y >= m_zone.top + m_zone.height)
		newPosition.y -= m_zone.height;

	_particle->SetPosition(newPosition);
}