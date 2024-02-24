#pragma once

#include <C78E/Core/Types.h>
#include <C78E/Assets/AssetLib.h>
#include <C78E/Assets/Mesh/Mesh.h>
#include <C78E/Assets/Material/Material.h>
#include <C78E/Assets/Texture/Texture.h>

namespace C78E {

	class Model {
	public:
		Model() = default;
		Model(const Asset<Mesh>& mesh, const Asset<Material>& material);
		Model(const Model& other) = default;
		~Model();

		static Ref<Model> create(std::string file);
		static std::vector<Ref<Model>> createAll(std::string file);
		
	public:
		Asset<Mesh> m_Mesh;
		Asset<Material> m_Material;
	};

}