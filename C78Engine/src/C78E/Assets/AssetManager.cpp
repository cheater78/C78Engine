#include <C78ePCH.h>
#include "AssetManager.h"

namespace C78E {

	AssetLib<Shader>		AssetManager::s_ShaderLib;
	AssetLib<Texture2D>		AssetManager::s_Texture2DLib;
	AssetLib<CubeMap>		AssetManager::s_CubeMapLib;
	AssetLib<Mesh>			AssetManager::s_MeshLib;
	AssetLib<Material>		AssetManager::s_MaterialLib;

}