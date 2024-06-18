#pragma once

#include <C78E.h>
#include <C78Elibs.h>

#include "EntityComponentUI.h"

namespace C78Editor::UI  {

	//class EntityListUI;
	//class EntityComponentInspectorUI;
	//
	//class EntityManagerUI {
	//	EntityManagerUI(C78E::Ref<C78E::SceneManager> sceneManager);
	//	EntityManagerUI(const EntityManagerUI& other) = delete;
	//	~EntityManagerUI();
	//
	//	void onImGuiMainMenuBar();
	//	void onImGuiRender();
	//
	//	void selectEntity(C78E::UUID entityID);
	//
	//protected:
	//	bool isSceneEmpty() const;
	//	bool isEntitySelected() const;
	//	void selectAnyEntity();
	//private:
	//	C78E::Ref<C78E::SceneManager> m_SceneManager;
	//	C78E::Entity m_ActiveEntity = C78E::Entity();
	//
	//	EntityListUI m_EntityListUI;
	//	EntityComponentInspectorUI m_EntityComponentInspectorUI;
	//
	//	friend class EntityListUI;
	//};
	//
	//class EntityListUI : public C78E::GUI::Window { //TODO: init Table
	//private:
	//	class EntityListTable : private C78E::GUI::Table {
	//	public:
	//		class EntityListEntry : private C78E::GUI::Table::TableEntry {
	//		public:
	//			EntityListEntry(C78E::Entity entity);
	//			~EntityListEntry();
	//
	//			virtual void content() override;
	//		private:
	//
	//		};
	//	public:
	//		EntityListTable();
	//		~EntityListTable();
	//
	//		virtual void content() override;
	//	private:
	//
	//	};
	//	
	//public:
	//	EntityListUI(C78E::Ref<C78E::SceneManager> sceneManager, const std::string& title = "EntityList", C78E::Gui::Window::Style style = {});
	//	~EntityListUI();
	//
	//	virtual void content() override;
	//private:
	//	C78E::Ref<C78E::SceneManager> m_SceneManager = nullptr;
	//
	//	EntityListTable m_EntityTable;
	//	std::unordered_map<C78E::UUID, C78E::Ref<EntityListTable::EntityListEntry>> m_EntityListEntries;
	//
	//
	//};
	//
	//class EntityComponentInspectorUI : public C78E::GUI::Window {
	//public:
	//	EntityComponentInspectorUI(C78E::Ref<C78E::SceneManager> sceneManager, const std::string& title = "EntityComponentInspector", C78E::Gui::Window::Style style = {});
	//	~EntityComponentInspectorUI();
	//
	//	virtual void content() override;
	//private:
	//	C78E::Ref<C78E::SceneManager> m_SceneManager = nullptr;
	//};

}
