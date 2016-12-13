#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	EngineCore engineCore;

	TextureManager& EngineCore::GetTextureManager()
	{
		return m_textureMng;
	}

	ColliderManager& EngineCore::GetColliderManager()
	{
		return m_colliderMng;
	}

	DialogManager& EngineCore::GetDialogManager()
	{
		return m_dialogManager;
	}

	TileSetManager& EngineCore::GetTileSetManager()
	{
		return m_tileSetManager;
	}
}