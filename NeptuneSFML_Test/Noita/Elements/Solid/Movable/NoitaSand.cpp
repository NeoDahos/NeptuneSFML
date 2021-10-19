#include "NoitaSand.h"

NoitaSand::NoitaSand(bool currentUpdateFlag) : NoitaMovableSolid(currentUpdateFlag)
{
	m_type = NoitaElementCommon::Type::Sand;
}