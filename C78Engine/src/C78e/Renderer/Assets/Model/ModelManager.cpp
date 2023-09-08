#include "C78ePCH.h"
#include "ModelManager.h"

namespace C78e {
	Ref<ModelManager> ModelManager::s_MainTextureManager;

	ModelManager::ModelManager()
	{
	}

	ModelManager::~ModelManager()
	{
	}

	void ModelManager::load(std::string file)
	{
		m_Textures[file] = createRef<Mesh>();
	}

	Ref<Mesh> ModelManager::get(std::string name)
	{
		return m_Textures.at(name);
	}

	Ref<Mesh> ModelManager::get_s(std::string file)
	{
		if (m_Textures.find(file) == m_Textures.end())
			load(file);
		return get(file);
	}



	void ModelManager::load(std::string name, Mesh mesh)
	{
		m_Textures[name] = createRef<Mesh>(mesh);
	}
}