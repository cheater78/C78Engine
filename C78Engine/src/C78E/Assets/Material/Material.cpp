#include "C78EPCH.h"
#include "Material.h"

#include <C78E/Assets/AssetManager.h>

namespace C78E {
	Ref<Material> Material::create(std::string filename)
	{ return create(filename, 0); }

	Ref<Material> Material::create(std::string filename, uint32_t index) {
		MaterialProperties materialProperties;
		uint32_t illuminationModel;
		MaterialTextures materialTextures;
		MaterialPropertiesPBRext materialPropertiesPBRext;
		MaterialTexturesPBRext materialTexturesPBRext;


		if (verifyMTL(filename)) {
			//TODO: parse wavefront mtl file

			C78_CORE_ASSERT(false, "Implement! -> Material.cpp");
			return createRef<Material>(materialProperties, illuminationModel, materialTextures, materialPropertiesPBRext, materialTexturesPBRext);
		}
		else if (false) {
			//other Material format
		}

		C78_CORE_ASSERT(false, "Material: Type not supported! File: " + filename);
		return nullptr;
	}

	std::vector<Ref<Material>> Material::createAll(std::string filename) {
		std::vector<Ref<Material>> materials;

		

		return materials;
	}

	Material::Material(MaterialProperties materialProperties, uint32_t illuminationModel, MaterialTextures materialTextures, MaterialPropertiesPBRext materialPropertiesPBRext, MaterialTexturesPBRext materialTexturesPBRext)
		: m_Material(materialProperties), m_IlluminationModel(illuminationModel), m_Textures(materialTextures), m_PBRMaterial(materialPropertiesPBRext), m_PBRTextures(materialTexturesPBRext)
	{ }

	Material::Material(MaterialProperties materialProperties, uint32_t illuminationModel, MaterialTextures materialTextures, MaterialPropertiesPBRext materialPropertiesPBRext, MaterialTexturesPBRext materialTexturesPBRext, Asset<Shader> shader)
		: m_Material(materialProperties), m_IlluminationModel(illuminationModel), m_Textures(materialTextures), m_PBRMaterial(materialPropertiesPBRext), m_PBRTextures(materialTexturesPBRext), m_Shader(shader)
	{ }

	void Material::setShader(std::string name) {
		m_Shader = AssetManager::getShaderAsset(name);
	}

	Asset<Shader>& Material::getShader() { return m_Shader; }

	Material::MaterialProperties& Material::getProperties() { return m_Material; }

	Material::MaterialTextures& Material::getTextures() { return m_Textures; }

	

	bool Material::verifyMTL(std::string file) {
		//TODO: impl parsable .mtl check
		C78_CORE_ASSERT(false, "Implement! -> Material.cpp");
		return false;
	}
}