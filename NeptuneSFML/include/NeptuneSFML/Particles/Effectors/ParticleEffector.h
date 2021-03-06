#ifndef PARTICLE_EFFECTOR_H
#define PARTICLE_EFFECTOR_H
#include <NeptuneSFML\Export.h>

namespace nep
{
	class NEPTUNE_API ParticleEffector
	{
	public:
		virtual void Apply(class Particle * const _particle) = 0;
		void SetActive(bool _activate = true)
		{
			m_isActive = _activate;
		}

	protected:
		bool m_isActive = true;
	};
}

#endif // PARTICLE_EFFECTOR_H