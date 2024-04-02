#pragma once

#include <C78E/Assets/Material/Material.h>
#include <C78E/Assets/Texture/Texture.h>

namespace tinyobj {
	struct material_t;
	struct texture_option_t;
}

namespace C78E {
	class Model;

	class TinyObjectLoader {
	public:
		TinyObjectLoader() = delete;
		TinyObjectLoader(const TinyObjectLoader& other) = delete;
		~TinyObjectLoader() = delete;

	public:
		static std::vector<Ref<Model>> loadAllModels(std::string file, std::string name = "");

	private:
		static Ref<Material> toMaterial(const tinyobj::material_t& material);
		static Material::MaterialProperties toMaterialProperties(const tinyobj::material_t& material);
		static Material::MaterialTextures toMaterialTextures(const tinyobj::material_t& material);
		static Material::MaterialPropertiesPBRext toMaterialPropertiesPBRext(const tinyobj::material_t& material);
		static Material::MaterialTexturesPBRext toMaterialTexturesPBRext(const tinyobj::material_t& material);
		static Texture::TextureOption toMaterialTextureOption(const tinyobj::texture_option_t& texOpt);
	};

}