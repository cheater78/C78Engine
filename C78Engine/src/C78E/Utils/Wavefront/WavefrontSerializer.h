#pragma once

#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Material/Material.h>
#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class WavefrontSerializer {
	public:
		static Ref<Asset::Group> importWavefront(const FilePath& source);
		static Ref<Asset::Group> importWavefront(const std::string& source);

		static bool importMesh(Ref<Mesh> mesh, Ref<Asset::Meta> meta, std::string_view source);
		static bool importMaterial(Ref<Material> mesh, Ref<Asset::Meta> meta, std::string_view source);
	private:

		

	};



}