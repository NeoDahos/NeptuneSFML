#include "NoitaImmovableSolid.h"

NoitaImmovableSolid::NoitaImmovableSolid(bool updateFlag) : NoitaSolid(updateFlag)
{
}

void NoitaImmovableSolid::Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag)
{
}

bool NoitaImmovableSolid::IsSwapPossible(NoitaElement* otherElement)
{
	return false;
}
