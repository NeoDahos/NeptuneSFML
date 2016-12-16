#include <NeptuneSFML\Particles\Effectors\ColorByVelocity.h>
#include <NeptuneSFML\Particles\Particle.h>
#include <NeptuneSFML\Tools.h>

void nep::ColorByVelocity::Init(const sf::Color & _startColor, const sf::Color & _endColor, float _minVelocity, float _maxVelocity)
{
	m_startColor = _startColor;
	m_endColor = _endColor;
	m_minVelocity = _minVelocity;
	m_maxVelocity = _maxVelocity;
}

void nep::ColorByVelocity::Apply(Particle * const _particle)
{
	float velocityMagnitudeSq = VectorLengthSq(_particle->GetVelocity());
	velocityMagnitudeSq = Clamp(velocityMagnitudeSq / SQUARE(m_minVelocity + m_maxVelocity), 0.f, 1.f);

	_particle->SetColor(LerpColor(m_startColor, m_endColor, velocityMagnitudeSq));
}