#include <NeptuneSFML\EngineCore.h>

namespace nep
{
	EngineCore engineCore;

	EngineCore::EngineCore()
	{
		m_fontManager.AddFont("default", "../Data/Font/arial.ttf");
	}

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

	FontManager& EngineCore::GetFontManager()
	{
		return m_fontManager;
	}
}