#ifndef LIFE_TIME_H
#define LIFE_TIME_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Particles\Effectors\ParticleEffector.h>

namespace nep
{
	class NEPTUNE_API LifeTime : public nep::ParticleEffector
	{
	public:
		void Init(float _lifeTime, bool _activated = true);

		virtual void Apply(class Particle * const _particle);

	private:
		float m_lifeTime;
	};
}

#endif // LIFE_TIME_H