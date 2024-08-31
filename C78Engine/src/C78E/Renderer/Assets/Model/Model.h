#pragma once

#include <C78E/Core/Types.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	/*
	* Model
	* consisting of parts, with each part containing a Mesh and its corresponding material
	* only stores AssetHandles which must be registered at the current AssetManager instance
	*/
	class Model : public Asset {
	public:
		struct ModelPart {
			AssetHandle m_Mesh = AssetHandle::invalid();
			AssetHandle m_Material = AssetHandle::invalid();
		};
	public:
		Model() = default;
		Model(const std::vector<ModelPart>& parts)
			: m_Parts(parts) { }
		Model(const Model& other) = default;
		~Model() { }
	public:
		virtual AssetType getType() { return Asset::AssetType::Model; };
		static AssetType getClassType() { return AssetType::Model; };
	public:
		std::vector<ModelPart> m_Parts; // list of pairs with Mesh and Material
	};

}