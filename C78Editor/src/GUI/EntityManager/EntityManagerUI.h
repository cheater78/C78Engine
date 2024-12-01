#pragma once

#include <C78E.h>

namespace C78Editor::GUI {
	
	class EntityManagerUI {
	public:
		EntityManagerUI(C78E::Ref<C78E::SceneManager> sceneManager);
		EntityManagerUI(const EntityManagerUI& other) = delete;
		~EntityManagerUI();
	
		void onImGuiMainMenuBar();//
		void onImGuiRender();
	
		void selectEntity(C78E::UUID entityID);
	
	protected:
		bool isEntitySelected() const;//

	private:
		void drawEntityManager(C78E::Ref<C78E::SceneManager> sceneManager);
		void drawEntityComponentManager(C78E::Ref<C78E::SceneManager> sceneManager);

		void drawEntityListEntry(C78E::Entity entity);
		void drawEntityComponentList(C78E::Entity entity);

		template<typename T>
		void drawManagedAddComponentEntry(const std::string& entryName);

		template<typename T, typename UIFunction>
		void drawComponent(const std::string& name, C78E::Entity entity, UIFunction uiFunction);

	private:
		C78E::WRef<C78E::SceneManager> m_SceneManager;
		C78E::Entity m_ActiveEntity;
	};
	
	

	

}
