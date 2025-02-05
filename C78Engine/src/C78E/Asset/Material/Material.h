#pragma once

#include <C78E/Asset/Asset.h>
#include <C78E/Renderer/API/Texture.h>
#include <C78E/Asset/Scene/Entity/Component/Uniform.h>

namespace C78E {

	class Material : public Asset {
	public:
		enum class Type : uint8_t {
			Material = 0,
			BasicMaterial,
			PBRMaterial,
		};

		enum class IlluminationModel : uint8_t {
			ColoredNoAmbient,											//0. Color on and Ambient off
			ColoredAmbient,												//1. Color on and Ambient on
			Highlight,													//2. Highlight on
			ReflectionRaytrace,											//3. Reflection on and Ray trace on
			TransparentReflectionRaytrace,								//4. Transparency: Glass on, Reflection : Ray trace on
			ReflectionFresnelRaytrace,									//5. Reflection : Fresnel on and Ray trace on
			TransparencyRefractionReflectionNoFresnelRaytrace,			//6. Transparency : Refraction on, Reflection : Fresnel off and Ray trace on
			TransparencyRefractionReflectionFresnelRaytrace,			//7. Transparency : Refraction on, Reflection : Fresnel on and Ray trace on
			ReflectionNoRaytrace,										//8. Reflection on and Ray trace off
			TransparencyGlassReflectionNoRaytrace,						//9. Transparency : Glass on, Reflection : Ray trace off
			InvisShadows,												//10. Casts shadows onto invisible surfaces
		};
	public:
		static Ref<Material> create(Material::Type materialType = Type::Material);
	public:
		Material() {}
		Material(AssetHandle shader, IlluminationModel illuminationModel) : m_Shader(shader), m_IlluminationModel(illuminationModel) { }
		~Material() = default;

	public:
		virtual Type getMaterialType() const { return Type::Material; };
		static Type getMaterialClassType() { return Type::Material; };

	public:
		virtual Asset::Type getType() const override final { return Asset::Type::Material; };
		static Asset::Type getClassType() { return Asset::Type::Material; };
	public:
		AssetHandle m_Shader = AssetHandle::invalid();
		IlluminationModel m_IlluminationModel;
		std::vector<AssetHandle> m_Textures;
	};



	class BasicMaterial : public Material {
	public:
		struct Properties {
			glm::vec3 ambient = { 1.f, 1.f, 1.f };						// Ka: Ambient Color Reaction [0,1]�
			alignas(16) glm::vec3 diffuse = { 1.f, 1.f, 1.f };			// Kd: Diffuse Color Reaction [0,1]�
			alignas(16) glm::vec3 specular = { 1.f, 1.f, 1.f };			// Ks: Specular Color Reaction [0,1]�
			alignas(16) glm::vec3 transmittance = { 1.f, 1.f, 1.f };	// Tf: Transmission Filter [0,1]�
			alignas(16) glm::vec3 emission = { 0.f, 0.f, 0.f };			// Ke: Emissive Color [0,1]�
			alignas(16) float shininess = 100.f;						// Ns: Specular Highlight(specular exponent) [0,1000]
			float ior = 1.f;											// Ni: Optical Density -> Index Of Refraction [0.001,10] (e.g. glass 1.5)
			float dissolve = 1.0f;										// d:  Dissolve Factor -> Opacity [0,1]
		};

		struct Textures {
			AssetHandle ambient = AssetHandle::invalid();				// map_Ka. For ambient or ambient occlusion.
			AssetHandle diffuse = AssetHandle::invalid();				// map_Kd
			AssetHandle specular = AssetHandle::invalid();				// map_Ks
			AssetHandle specularHighlight = AssetHandle::invalid();		// map_Ns
			AssetHandle bump = AssetHandle::invalid();					// map_bump, map_Bump, bump
			AssetHandle displacement = AssetHandle::invalid();			// disp
			AssetHandle alpha = AssetHandle::invalid();					// map_d
			AssetHandle reflection = AssetHandle::invalid();			// refl
		};

	public:
		BasicMaterial() = default;
		BasicMaterial(AssetHandle shader, IlluminationModel illuminationModel, Properties materialProperties, Textures materialTextures)
			: Material(shader, illuminationModel), m_MaterialProperties(materialProperties), m_MaterialTextures(materialTextures) {
		}
		~BasicMaterial() = default;
	public:
		virtual Type getMaterialType() const { return Type::BasicMaterial; };
		static Type getMaterialClassType() { return Type::BasicMaterial; };
	public:
		static Asset::Type getClassType() { return Asset::Type::Material; };
	public:
		Properties m_MaterialProperties;
		Textures m_MaterialTextures;
	};



	class PBRMaterial : public Material {
	public:
		struct Properties {
			float roughness = 0;										//Pr: roughness [0, 1] default 0
			float metallic = 0;											//Pm: metallic [0, 1] default 0
			float sheen = 0;											//Ps: sheen [0, 1] default 0
			float clearcoat_thickness = 0;								//Pc: clearcoat thickness [0, 1] default 0
			float clearcoat_roughness = 0;								//Pcr: clearcoat roughness [0, 1] default 0
			float anisotropy = 0;										//aniso: anisotropy [0, 1] default 0
			float anisotropy_rotation = 0;								//anisor: anisotropy rotation [0, 1] default 0
		};

		struct Textures {
			AssetHandle roughness = AssetHandle::invalid();				// map_Pr: roughness
			AssetHandle metallic = AssetHandle::invalid();				// map_Pm: metallic
			AssetHandle sheen = AssetHandle::invalid();					// map_Ps: sheen
			AssetHandle emissive = AssetHandle::invalid();				// map_Ke: emissive
			AssetHandle normal = AssetHandle::invalid();				// norm: normal map(RGB components represent XYZ components of the surface normal)
		};

	public:
		PBRMaterial() = default;
		PBRMaterial(AssetHandle shader, IlluminationModel illuminationModel, Properties materialProperties, Textures materialTextures)
			: Material(shader, illuminationModel), m_MaterialProperties(materialProperties), m_MaterialTextures(materialTextures) {
		}
		~PBRMaterial() = default;
	public:
		virtual Type getMaterialType() const { return Type::PBRMaterial; };
		static Type getMaterialClassType() { return Type::PBRMaterial; };
	public:
		static Asset::Type getClassType() { return Asset::Type::Material; };
	public:
		Properties m_MaterialProperties;
		Textures m_MaterialTextures;
	};
	
}