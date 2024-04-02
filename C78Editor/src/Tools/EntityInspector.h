#pragma once
#include <C78E.h>
#include <C78Elibs.h>

namespace C78Editor {

	class EntityInspector {
	public:
		static void init();
		static void onImGuiRender(C78E::Ref<C78E::Scene> scene);

	private:
		static void showEntityList(std::string element, C78E::Ref<C78E::Scene> scene);
		static void showInspectorList(std::string element, C78E::Entity& entity);

		static void insID(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insTag(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insState(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insTransform(std::string element, C78E::Entity& entity, uint32_t indent = 0);

		static void insCamera(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		
		static void insAmbinentLight(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insDirectLight(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insPointLight(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insSpotLight(std::string element, C78E::Entity& entity, uint32_t indent = 0);

		static void insModel(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insMesh(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insMaterial(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insTexture(std::string element, C78E::Entity& entity, uint32_t indent = 0);
		static void insSkyBox(std::string element, C78E::Entity& entity, uint32_t indent = 0);

		static void insModel(std::string element, C78E::Asset<C78E::Model> model, uint32_t indent = 0);
		static void insMesh(std::string element, C78E::Asset<C78E::Mesh> mesh, uint32_t indent = 0);
		static void insMaterial(std::string element, C78E::Asset<C78E::Material> material, uint32_t indent = 0);
		static void insShader(std::string element, C78E::Asset<C78E::Shader> shader, uint32_t indent = 0);
		static void insTexture(std::string element, C78E::Asset<C78E::Texture2D> texture, uint32_t indent = 0);
		static void insCubeMap(std::string element, C78E::Asset<C78E::CubeMap> texture, uint32_t indent = 0);
		
		static void insMaterialTexture(std::string element, std::string name, std::string& texture, C78E::Texture::TextureOption option, uint32_t indent = 0);

		static std::string indentStr(std::string str, uint32_t ind = 0);

		static bool& shouldShow(std::string element);
		static void toggleShow(std::string element);

	private:
		static uint32_t s_ID;
		static std::unordered_map<std::string, bool> s_ShowElement;
	};

}