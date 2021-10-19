#pragma once

#include "../NoitaSolid.h"

class NoitaImmovableSolid : public NoitaSolid
{
public:
	NoitaImmovableSolid(bool updateFlag);

	void Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag) override;
	bool IsSwapPossible(NoitaElement* otherElement) override;
};