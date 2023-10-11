#pragma once
#include <C78E.h>

class EnttInspector {
public:
	static void onImGuiRender(C78E::Ref<C78E::Scene> scene);
private:
	static uint32_t s_ID;
	static bool s_showInspector;

	static void showEntityList(C78E::Ref<C78E::Scene> scene);
	static void showInspectorList(C78E::Ref<C78E::Scene> scene);
};