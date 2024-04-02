#pragma once

#include <C78e/Core/Types.h>
#include <C78E/Assets/Asset/Asset.h>
#include <C78E/Assets/Texture/Texture.h>

namespace C78E {

	class Material : public Asset {
	public:
		struct MaterialProperties {
						glm::vec3 ambient = { 1.f, 1.f, 1.f };			// Ka: Ambient Color Reaction [0,1]³
			alignas(16) glm::vec3 diffuse = { 1.f, 1.f, 1.f };			// Kd: Diffuse Color Reaction [0,1]³
			alignas(16) glm::vec3 specular = { 1.f, 1.f, 1.f };			// Ks: Specular Color Reaction [0,1]³
			alignas(16) glm::vec3 transmittance = { 1.f, 1.f, 1.f };	// Tf: Transmission Filter [0,1]³
			alignas(16) glm::vec3 emission = { 0.f, 0.f, 0.f };			// Ke: Emissive Color [0,1]³
			alignas(16) float shininess = 100.0;						// Ns: Specular Highlight [0,1000]
						float ior = 1.f;								// Ni: Optical Density -> Index Of Refraction [0.001,10] (e.g. glass 1.5)
						float dissolve = 1.0f;							// d:  Dissolve Factor -> Opacity [0,1]
		};

		struct MaterialTextures {
			std::string ambient;             // map_Ka. For ambient or ambient occlusion.
			std::string diffuse;             // map_Kd
			std::string specular;            // map_Ks
			std::string specularHighlight;	 // map_Ns
			std::string bump;                // map_bump, map_Bump, bump
			std::string displacement;        // disp
			std::string alpha;               // map_d
			std::string reflection;          // refl

			Texture::TextureOption ambientOpt;
			Texture::TextureOption diffuseOpt;
			Texture::TextureOption specularOpt;
			Texture::TextureOption specularHighlightOpt;
			Texture::TextureOption bumpOpt;
			Texture::TextureOption displacementOpt;
			Texture::TextureOption alphaOpt;
			Texture::TextureOption reflectionOpt;
		};

		struct MaterialPropertiesPBRext {
			float roughness;            // [0, 1] default 0
			float metallic;             // [0, 1] default 0
			float sheen;                // [0, 1] default 0
			float clearcoat_thickness;  // [0, 1] default 0
			float clearcoat_roughness;  // [0, 1] default 0
			float anisotropy;           // aniso. [0, 1] default 0
			float anisotropy_rotation;  // anisor. [0, 1] default 0
		};

		struct MaterialTexturesPBRext {
			std::string roughness_texname;  // map_Pr
			std::string metallic_texname;   // map_Pm
			std::string sheen_texname;      // map_Ps
			std::string emissive_texname;   // map_Ke
			std::string normal_texname;     // norm. For normal mapping.

			Texture::TextureOption roughnessOpt;
			Texture::TextureOption metallicOpt;
			Texture::TextureOption sheenOpt;
			Texture::TextureOption emissiveOpt;
			Texture::TextureOption normalOpt;
		};

	public:
		Material() = default;
		Material(
			AssetHandle shader,
			MaterialProperties materialProperties,
			uint32_t illuminationModel,
			MaterialTextures materialTextures,
			MaterialPropertiesPBRext materialPropertiesPBRext = {},
			MaterialTexturesPBRext materialTexturesPBRext = {}
		)
			: m_Shader(shader),
			m_MaterialProperties(materialProperties),
			m_IlluminationModel(illuminationModel),
			m_Textures(materialTextures),
			m_PBRMaterialProperties(materialPropertiesPBRext),
			m_PBRTextures(materialTexturesPBRext)
		{ }
		Material(const Material&) = default;
		~Material() = default;
	public:
		virtual AssetType getType() { return Asset::AssetType::Material; };

	public:
		AssetHandle m_Shader;
		MaterialProperties m_MaterialProperties;
		uint32_t m_IlluminationModel;
		MaterialTextures m_Textures;
		MaterialPropertiesPBRext m_PBRMaterialProperties;
		MaterialTexturesPBRext m_PBRTextures;
	};
}