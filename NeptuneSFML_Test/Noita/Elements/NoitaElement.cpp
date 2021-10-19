#include "NoitaElement.h"

NoitaElement::NoitaElement(bool currentUpdateFlag)
{
	m_updateFlag = currentUpdateFlag;
}

void NoitaElement::Update(NoitaParticleChunk& currentChunk, int x, int y, int index, bool updateFlag)
{
}

bool NoitaElement::IsSwapPossible(NoitaElement* otherElement)
{
	return true;
}

NoitaElementCommon::Type NoitaElement::GetType() const
{
	return m_type;
}
