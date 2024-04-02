#pragma once

#include <C78E/Core/Types.h>
#include <C78E/Assets/Asset/Asset.h>

namespace C78E {

	class Model : public Asset {
	public:
		Model() = default;
		Model(const AssetHandle mesh, const AssetHandle material)
			: m_Mesh(mesh), m_Material(material) { }
		Model(const Model& other) = default;
		~Model() { }
		
	public:
		AssetHandle m_Mesh = 0;
		AssetHandle m_Material = 0;
	};

}