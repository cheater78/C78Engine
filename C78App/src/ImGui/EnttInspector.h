#pragma once
#include <C78E.h>

namespace C78Editor {

	class EnttInspector {
	public:
		static void init();
		static void onImGuiRender(C78E::Ref<C78E::Scene> scene);

	private:
		static void showEntityList(C78E::Ref<C78E::Scene> scene);
		static void showInspectorList(C78E::Entity& entity);
	private:
		static uint32_t s_ID;
		static bool s_showInspector;

		static bool s_ShowDetailedTransformComponent;

		static bool s_ShowDetailedCameraComponent;

		static bool s_ShowDetailedAmbientLightComponent;
		static bool s_ShowDetailedDirectLightComponent;
		static bool s_ShowDetailedPointLightComponent;
		static bool s_ShowDetailedSpotLightComponent;

		static bool s_ShowDetailedMeshComponent;
		static bool s_ShowDetailedMaterialComponent;
		static bool s_ShowDetailedTextureComponent;
	};

}