#include "EntityManagerUI.h"

namespace C78Editor::UI {

	
	/*
	* EntityManagerUI
	*/
	//EntityManagerUI::EntityManagerUI(C78E::Ref<C78E::SceneManager> sceneManager)
	//	: m_SceneManager(sceneManager), m_EntityListUI(sceneManager), m_EntityComponentInspectorUI(sceneManager) {
	//}
	//
	//EntityManagerUI::~EntityManagerUI() { }
	//
	//void EntityManagerUI::onImGuiMainMenuBar() {
	//}
	//
	//void EntityManagerUI::onImGuiRender() {
	//	if (!m_SceneManager->hasActiveScene()) return;
	//	m_EntityListUI.show();
	//	if (!isSceneEmpty())
	//		m_EntityComponentInspectorUI.show();
	//}
	//
	//void EntityManagerUI::selectEntity(C78E::UUID entityID) {
	//	m_Scene->getEntityByUUID(entityID);
	//}
	//
	//bool EntityManagerUI::isSceneEmpty() const {
	//	if (!m_Scene) return true;
	//	for (auto entts : m_Scene->getAllEntitiesWith<C78E::IDComponent, C78E::TagComponent, C78E::TransformComponent>())
	//		return false;
	//	return true;
	//}
	//
	//bool EntityManagerUI::isEntitySelected() const {
	//	return m_ActiveEntity != C78E::Entity();
	//}
	//
	//void EntityManagerUI::selectAnyEntity() {
	//	if (isSceneEmpty()) {
	//		C78_EDITOR_ERROR("EntityManagerUI::selectAnyEntity: Cannot select Entity, the active scene is empty!");
	//		return;
	//	}
	//	m_ActiveEntity = C78E::Entity(m_Scene->getAllEntitiesWith<C78E::IDComponent, C78E::TagComponent, C78E::TransformComponent>().front(), m_Scene.get());
	//}
	//
	//
	//
	//
	//
	//
	///*
	//* EntityListUI
	//*/
	//EntityListUI::EntityListUI(C78E::Ref<C78E::SceneManager> entityManager, const std::string& title, C78E::Gui::Window::Style style)
	//	: C78E::Gui::Window(title, style), m_SceneManager(entityManager) {
	//}
	//
	//EntityListUI::~EntityListUI() { }
	//
	//void EntityListUI::windowContent() {
	//	//TODO: EntityList -> Table n stuff
	//
	//	for (auto entt : m_SceneManager->getActiveScene()->getAllEntitiesWith<C78E::IDComponent, C78E::TagComponent, C78E::TransformComponent>()) {
	//		C78E::Entity entity(entt, m_SceneManager->getActiveScene().get());
	//		C78E::UUID uuid = entity.getUUID();
	//
	//		if (!m_EntityListEntries.contains(uuid)) {
	//			m_EntityListEntries[uuid] = C78E::createRef<EntityListEntry>(entity); //TODO create actual Entity Entry
	//		}
	//
	//		m_EntityListEntries[uuid]->show();
	//	}
	//
	//}
	//
	//
	//
	///*
	//* EntityComponentInspectorUI
	//*/
	//EntityComponentInspectorUI::EntityComponentInspectorUI(C78E::Ref<C78E::SceneManager> entityManager, const std::string& title, C78E::Gui::Window::Style style) {
	//
	//}
	//
	//EntityComponentInspectorUI::~EntityComponentInspectorUI() {
	//
	//}
	//
	//void EntityComponentInspectorUI::windowContent() {
	//
	//}
}


