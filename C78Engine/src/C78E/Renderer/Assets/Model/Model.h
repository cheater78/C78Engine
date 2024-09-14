#pragma once

#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Material/Material.h>

namespace C78E {

	/*
	* Model
	* consisting of a Mesh and its corresponding material
	* only stores AssetHandles which must be registered at the current AssetManager instance
	*/
	class Model : public Asset {
	public:
		Model(Ref<C78E::Mesh> mesh, Ref<C78E::Material> material) : m_Mesh(mesh), m_Material(material) { }
		Model(const Model& other) = delete;
		~Model() = default;

		Ref<C78E::Mesh> mesh() { return m_Mesh; }
		Ref<C78E::Material> material() { return m_Material; }
	public:
		virtual AssetType getType() const override { return Asset::AssetType::Model; };
		static AssetType getClassType() { return AssetType::Model; };
	private:
		Ref<C78E::Mesh> m_Mesh;
		Ref<C78E::Material> m_Material;
	};

}