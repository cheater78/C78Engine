#pragma once

#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Material/Material.h>
#include <C78E/Renderer/API/Texture.h>

namespace tinyobj {
	struct material_t;
	struct texture_option_t;
}

namespace C78E {

	class WavefrontLoader {
	public:
		struct WavefrontModel {
			std::map<uint64_t, uint64_t> parts;
			std::map<uint64_t, Ref<Mesh>> meshes;
			std::map<uint64_t, std::string> meshNames;
			std::map<uint64_t, Ref<Material>> materials;
			std::map<uint64_t, std::string> materialNames;
		};
		struct WavefrontMaterial {
			std::map<uint64_t, Ref<Material>> materials;
			std::map<uint64_t, std::string> materialNames;
		};
	public:
		static Ref<WavefrontModel> loadModel(FilePath file);
		static Ref<WavefrontMaterial> loadMaterial(FilePath file);
	private:
		WavefrontLoader() = delete;
		WavefrontLoader(const WavefrontLoader& other) = delete;
		~WavefrontLoader() = delete;
	private:
		static Ref<Material> toMaterial(const tinyobj::material_t& material);
		static Material::MaterialProperties toMaterialProperties(const tinyobj::material_t& material);
		static Material::MaterialTextures toMaterialTextures(const tinyobj::material_t& material);
		static Material::MaterialPropertiesPBRext toMaterialPropertiesPBRext(const tinyobj::material_t& material);
		static Material::MaterialTexturesPBRext toMaterialTexturesPBRext(const tinyobj::material_t& material);
		static Texture::TextureOption toMaterialTextureOption(const tinyobj::texture_option_t& texOpt);
	};

}