#ifndef PARTICLE_EFFECTOR_H
#define PARTICLE_EFFECTOR_H

namespace nep
{
	class ParticleEffector
	{
	public:
		virtual void Apply(class Particle * const _particle) = 0;
	};
}

#endif // PARTICLE_EFFECTOR_H