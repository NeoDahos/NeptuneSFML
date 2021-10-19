#include "NoitaMovableSolid.h"

NoitaMovableSolid::NoitaMovableSolid(bool currentUpdateFlag) : NoitaSolid(currentUpdateFlag)
{
}


void NoitaMovableSolid::Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag)
{
	if (m_updateFlag == updateFlag)
		return;

	m_updateFlag = !m_updateFlag;

	NoitaElementCommon::ChunkElement destinationChunkElement = currentChunk.GetChunkElement(x, y - 1);

	if (destinationChunkElement.m_element->m_type == )

		if (AddChange(x, y - 1, index, NoitaParticle::SandSwapAllowed))
			return;

	AddChange(x - 1, y - 1, index, NoitaParticle::SandSwapAllowed);
	AddChange(x + 1, y - 1, index, NoitaParticle::SandSwapAllowed);
}

bool NoitaMovableSolid::IsSwapPossible(NoitaElement* otherElement)
{
	return otherElement->GetType() == NoitaElementCommon::Type::Empty || otherElement->GetType() == NoitaElementCommon::Type::Water;
}