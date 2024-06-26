#pragma once

#include <C78e/Core/Types.h>
#include <C78E/Renderer/Assets/Asset/Asset.h>
#include <C78E/Renderer/API/Texture.h>

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
			AssetHandle ambient = 0;             // map_Ka. For ambient or ambient occlusion.
			AssetHandle diffuse = 0;             // map_Kd
			AssetHandle specular = 0;            // map_Ks
			AssetHandle specularHighlight = 0;	 // map_Ns
			AssetHandle bump = 0;                // map_bump, map_Bump, bump
			AssetHandle displacement = 0;        // disp
			AssetHandle alpha = 0;               // map_d
			AssetHandle reflection = 0;          // refl
		};

		struct MaterialPropertiesPBRext {
			float roughness = 0;            // [0, 1] default 0
			float metallic = 0;             // [0, 1] default 0
			float sheen = 0;                // [0, 1] default 0
			float clearcoat_thickness = 0;  // [0, 1] default 0
			float clearcoat_roughness = 0;  // [0, 1] default 0
			float anisotropy = 0;           // [0, 1] default 0
			float anisotropy_rotation = 0;  // [0, 1] default 0
		};

		struct MaterialTexturesPBRext {
			AssetHandle roughness = 0;		// map_Pr
			AssetHandle metallic = 0;		// map_Pm
			AssetHandle sheen = 0;			// map_Ps
			AssetHandle emissive = 0;		// map_Ke
			AssetHandle normal = 0;			// norm. For normal mapping.
		};

	public:
		Material() = default;
		Material(
			AssetHandle shader,
			uint32_t illuminationModel,
			MaterialProperties materialProperties,
			MaterialTextures materialTextures,
			MaterialPropertiesPBRext materialPropertiesPBRext = {},
			MaterialTexturesPBRext materialTexturesPBRext = {}
		)
			: m_Shader(shader),
			m_IlluminationModel(illuminationModel),
			m_MaterialProperties(materialProperties),
			m_MaterialTextures(materialTextures),
			m_MaterialPropertiesPBRext(materialPropertiesPBRext),
			m_MaterialTexturesPBRext(materialTexturesPBRext)
		{ }
		Material(const Material&) = default;
		~Material() = default;
	public:
		virtual AssetType getType() { return Asset::AssetType::Material; };

	public:
		AssetHandle m_Shader;
		uint32_t m_IlluminationModel;
		MaterialProperties m_MaterialProperties;
		MaterialTextures m_MaterialTextures;
		MaterialPropertiesPBRext m_MaterialPropertiesPBRext;
		MaterialTexturesPBRext m_MaterialTexturesPBRext;
	};
}