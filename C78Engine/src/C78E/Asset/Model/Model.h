#pragma once

#include <C78E/Asset/Mesh/Mesh.h>
#include <C78E/Asset/Material/Material.h>


namespace C78E {

	/**
	 * @brief Model stores Mesh and Material AssetHandles for Rendering
	 */
	class Model : public Asset {
	public:
		struct DrawableMesh : public std::pair<AssetHandle, AssetHandle> {
			DrawableMesh(AssetHandle mesh, AssetHandle material) : std::pair<AssetHandle, AssetHandle>(mesh, material) {}
		};
		using DrawableMeshes = std::vector<DrawableMesh>;

		static Ref<Model> create(const DrawableMesh& drawableMesh);
	public:
		virtual ~Model() override = default;

		DrawableMeshes& getDrawableMeshes() { return m_DrawableMeshes; }
		const DrawableMeshes& getDrawableMeshes() const { return m_DrawableMeshes; }

	protected:
		DrawableMeshes m_DrawableMeshes;
	};

}