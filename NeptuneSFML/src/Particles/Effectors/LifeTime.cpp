#include <NeptuneSFML\Particles\Effectors\LifeTime.h>
#include <NeptuneSFML\Particles\Particle.h>

namespace nep
{
	void LifeTime::Init(float _lifeTime, bool _activated)
	{
		m_lifeTime = _lifeTime;
		m_isActive = _activated;
	}

	void LifeTime::Apply(Particle * const _particle)
	{
		if (m_isActive)
		{
			if (_particle->GetLifeTime() >= m_lifeTime)
				_particle->Kill();
		}
	}
}