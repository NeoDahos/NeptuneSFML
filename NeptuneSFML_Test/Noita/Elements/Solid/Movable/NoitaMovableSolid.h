#pragma once

#include "../NoitaSolid.h"

class NoitaMovableSolid : public NoitaSolid
{
public:
	NoitaMovableSolid(bool currentUpdateFlag);

	void Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag) override;
	bool IsSwapPossible(NoitaElement* otherElement) override;
};