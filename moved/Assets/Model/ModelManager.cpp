#include "C78EPCH.h"
#include "ModelManager.h"

namespace C78E {
	Ref<ModelManager> ModelManager::s_ModelManager;

	void ModelManager::add(const std::string& name, const Ref<Mesh>& mesh)
	{
		C78_CORE_ASSERT(!exists(name), "Mesh already exists!");
		mesh.get()->setName(name);
		m_Meshes[name] = mesh;
	}

	void ModelManager::add(const Ref<Mesh>& mesh)
	{
		auto& name = mesh->getName();
		add(name, mesh);
	}

	Ref<Mesh> ModelManager::load(const std::string& filepath)
	{
		auto shader = createRef<Mesh>(filepath);
		add(shader);
		return shader;
	}

	Ref<Mesh> ModelManager::load(const std::string& name, const std::string& filepath)
	{
		auto mesh = createRef<Mesh>(filepath);
		add(name, mesh);
		return mesh;
	}

	Ref<Mesh> ModelManager::load(const std::string& name, Mesh mesh) {
		auto meshRef = createRef<Mesh>(mesh);
		add(name, meshRef);
		return meshRef;
	}

	Ref<Mesh> ModelManager::get(const std::string& name)
	{
		C78_CORE_ASSERT(exists(name), "Mesh not found!");
		return m_Meshes[name];
	}

	bool ModelManager::exists(const std::string& name) const
	{
		return m_Meshes.find(name) != m_Meshes.end();
	}

}