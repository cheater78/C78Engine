#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	/*
	* 
	* <COMPACT>
	* Attribute(AssetHandle) = AssetMeta.name [ view | change ]
	* 
	* 
	* <VIEW>
	* Attribute(AssetHandle) = AssetMeta.name [ view | change ]
	*	Name = AssetMeta.name
	*	Source = AssetMeta.fileSource
	*	**attr** = **value**
	* 
	* <CHANGE>
	* Attribute(AssetHandle) = AssetMeta.name [ view | change ]
	* //TODO
	*/

	class EntityInspector {
	public:
		static void init();
		static void onImGuiRender(C78E::Ref<C78E::Scene> scene);

	private:
		// Sub Systems
		static void showEntityList(std::string element, C78E::Ref<C78E::Scene> scene);
		static void showInspectorList(std::string element, C78E::Entity& entity);

		// Generic ImGui 2 Column Attribute : Value entries
		template <typename T>
		static void insData(std::string element, std::string title, const T& data, uint32_t indent = 0, ImGuiTableRowFlags rowFlags = 0);

		template <typename T>
		static void insDataOptToggleElement(std::string element, std::string title, const T& data, uint32_t indent = 0, ImGuiTableRowFlags rowFlags = 0);

		template <typename T>
		static void insDataOptToggleData(std::string element, std::string title, T& data, const T& state0, const T& state1, uint32_t indent = 0, ImGuiTableRowFlags rowFlags = 0);

		// Components
		static void insIDComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insTagComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insStateComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insTransformComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);

		static void insCameraComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		
		static void insAmbinentLightComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0); // maybe Deprecated
		static void insDirectLightComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0); // maybe Deprecated
		static void insPointLightComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0); // maybe Deprecated
		static void insSpotLightComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0); // maybe Deprecated

		static void insModelComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insMeshComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);  // maybe Deprecated
		static void insMaterialComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0); // maybe Deprecated
		static void insTextureComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insSkyBoxComponent(std::string element, C78E::Entity& entity, uint32_t indent = 0);

		// Assets
		static void insAsset(std::string element, std::string title, C78E::AssetHandle assetHandle, uint32_t indent = 0);

		static void insModel(std::string element, C78E::AssetHandle modelHandle, uint32_t indent = 0);
		static void insMesh(std::string element, C78E::AssetHandle meshHandle, uint32_t indent = 0);
		static void insMaterial(std::string element, C78E::AssetHandle materialHandle, uint32_t indent = 0);
		static void insShader(std::string element, C78E::AssetHandle shaderHandle, uint32_t indent = 0);
		static void insTexture(std::string element, C78E::AssetHandle textureHandle, uint32_t indent = 0);

		// Helpers
		static std::string indentStr(std::string str, uint32_t ind = 0);

		// Element System
		static bool& shouldShow(std::string element);
		static void toggleShow(std::string element);

	private:
		static uint32_t s_ID;
		static std::unordered_map<std::string, bool> s_ShowElement;
	};

}