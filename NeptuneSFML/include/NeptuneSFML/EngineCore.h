#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H
#include <NeptuneSFML\Export.h>

#include <NeptuneSFML\Manager\TextureManager.h>
#include <NeptuneSFML\Manager\ColliderManager.h>
#include <NeptuneSFML\Manager\DialogManager.h>
#include <NeptuneSFML\Manager\TileSetManager.h>
#include <NeptuneSFML\Manager\FontManager.h>

#include <SFML\System\NonCopyable.hpp>

namespace nep
{
	class NEPTUNE_API EngineCore : public sf::NonCopyable
	{
	public:
		EngineCore();

		TextureManager& GetTextureManager();
		ColliderManager& GetColliderManager();
		DialogManager& GetDialogManager();
		TileSetManager& GetTileSetManager();
		FontManager& GetFontManager();

	private:
		TextureManager m_textureMng;
		ColliderManager m_colliderMng;
		DialogManager m_dialogManager;
		TileSetManager m_tileSetManager;
		FontManager m_fontManager;
	};

	extern NEPTUNE_API EngineCore engineCore;

#define TextureMng engineCore.GetTextureManager()
#define ColliderMng engineCore.GetColliderManager()
#define DialogMng engineCore.GetDialogManager()
#define TileSetMng engineCore.GetTileSetManager()
#define FontMng engineCore.GetFontManager()
}

#endif // ENGINE_CORE_H