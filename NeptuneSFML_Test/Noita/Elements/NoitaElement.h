#pragma once
#include "../NoitaParticleChunk.h"
#include "NoitaElementCommon.h"

class NoitaElement
{
public:
	NoitaElement(bool currentUpdateFlag);
	virtual ~NoitaElement() {};

	virtual void Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag);
	virtual bool IsSwapPossible(NoitaElement* otherElement);

	NoitaElementCommon::Type GetType() const;

protected:
	NoitaElementCommon::Type m_type{ NoitaElementCommon::Type::Empty };
	bool m_updateFlag;
};