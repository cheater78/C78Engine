#include "EntityManagerUI.h"
#include "../uitils.h"

namespace C78Editor::GUI {

	
	/*
	* EntityManagerUI
	*/
	EntityManagerUI::EntityManagerUI(C78E::Ref<C78E::SceneManager> sceneManager)
		: m_SceneManager{ sceneManager }, m_ActiveEntity{ entt::null, nullptr } {
	}
	
	EntityManagerUI::~EntityManagerUI() { }
	
	void EntityManagerUI::onImGuiMainMenuBar() { }
	
	void EntityManagerUI::onImGuiRender() {
		if (auto sceneManager = m_SceneManager.lock()) {

			drawEntityManager(sceneManager);
			drawEntityComponentManager(sceneManager);

		}
		else {
			C78_EDITOR_ERROR("EntityManagerUI::onImGuiRender: called without m_SceneManager!");
		}
	}
	
	void EntityManagerUI::selectEntity(C78E::UUID entityID) {
		if (auto sceneManager = m_SceneManager.lock()) {
			if (!sceneManager->hasActiveScene()) return;
			sceneManager->getActiveScene()->getEntityByUUID(entityID);
		}
		else {
			C78_EDITOR_ERROR("EntityManagerUI::selectEntity: called without m_SceneManager!");
		}
	}

	bool EntityManagerUI::isEntitySelected() const {
		return m_ActiveEntity;
	}
	
	void EntityManagerUI::drawEntityManager(C78E::Ref<C78E::SceneManager> sceneManager) {
		ImGui::Begin("Entity Manager");
		if (sceneManager->hasActiveScene()) {
			auto scene = sceneManager->getActiveScene();

			if (ImGui::Button("Create Empty Entity"))
				scene->createEntity("Empty Entity");

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

			auto& meta = sceneManager->getProjectManager()->getActiveProject()->getEditorAssetManager()->getMeta(sceneManager->getActiveSceneHandle());

			std::string sceneName = meta.name;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)scene->m_AssetHandle, flags | ImGuiTreeNodeFlags_DefaultOpen, sceneName.c_str());

			if (opened) {
				scene->forEachEntity([&](auto entityID)
					{
						C78E::Entity entity{ entityID , scene.get() };
						drawEntityListEntry(entity);
					});

				ImGui::TreePop();
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_ActiveEntity = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1)) {
				if (ImGui::MenuItem("Create Empty Entity"))
					scene->createEntity("Empty Entity");

				ImGui::EndPopup();
			}
			

		}
		else {
			ImGui::Text("No Active Scene!");
		}
		ImGui::End();
	}

	void EntityManagerUI::drawEntityComponentManager(C78E::Ref<C78E::SceneManager> sceneManager) {
		ImGui::Begin("Entity Component Manager");
		drawEntityComponentList(m_ActiveEntity);
		ImGui::End();
	}

	void EntityManagerUI::drawEntityListEntry(C78E::Entity entity) {
		if (auto sceneManager = m_SceneManager.lock()) {
			auto& tag = entity.getComponent<C78E::TagComponent>().tag;

			ImGuiTreeNodeFlags flags = ((m_ActiveEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
			if (ImGui::IsItemClicked()) {
				m_ActiveEntity = entity;
			}

			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem("Delete Entity"))
					entityDeleted = true;

				ImGui::EndPopup();
			}

			if (opened) {
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
				if (ImGui::TreeNodeEx(("ID: " + std::to_string(entity.getUUID())).c_str(), flags)) {
					if (ImGui::IsItemClicked())
						m_ActiveEntity = entity;
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx(("Tag: " + entity.getTag()).c_str(), flags)) {
					if (ImGui::IsItemClicked())
						m_ActiveEntity = entity;
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx(std::string("Enable: " + std::string((entity.getComponent<C78E::StateComponent>().enable) ? "True" : "False")).c_str(), flags)) {
					if (ImGui::IsItemClicked())
						m_ActiveEntity = entity;
					ImGui::TreePop();
				}

				if (ImGui::TreeNodeEx("Transform", flags)) {
					if (ImGui::IsItemClicked())
						m_ActiveEntity = entity;
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (entityDeleted) {
				sceneManager->getActiveScene()->destroyEntity(entity);
				if (m_ActiveEntity == entity)
					m_ActiveEntity = {};
			}
		}
		else {
			C78_EDITOR_ERROR("EntityManagerUI::selectAnyEntity: called without m_SceneManager!");
		}
	}

	void EntityManagerUI::drawEntityComponentList(C78E::Entity entity) {
		if (auto sceneManager = m_SceneManager.lock()) {
			if (!sceneManager->hasActiveProject()) {
				ImGui::Text("No Active Project!");
				return;
			}
			if (!sceneManager->hasActiveScene()) {
				ImGui::Text("No Active Scene!");
				return;
			}
			if (!entity) {
				ImGui::Text("No Active Entity!");
				return;
			}

			if (entity.hasComponent<C78E::StateComponent>()) {
				bool* enable = &entity.getComponent<C78E::StateComponent>().enable;
				ImGui::Checkbox("##Entity State", enable);
				ImGui::SameLine();
			}


			if (entity.hasComponent<C78E::TagComponent>()) {
				auto& tag = entity.getComponent<C78E::TagComponent>().tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
				if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
					tag = std::string(buffer);
				}
			}

			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent")) {
				drawManagedAddComponentEntry<C78E::CameraComponent>("Camera");
				drawManagedAddComponentEntry<C78E::SpriteRendererComponent>("Sprite");

				ImGui::EndPopup();
			}

			ImGui::PopItemWidth();

			drawComponent<C78E::TransformComponent>("Transform", entity, [](C78E::TransformComponent& component) {
				drawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				drawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				drawVec3Control("Scale", component.Scale, 1.0f);
			});

			drawComponent<C78E::CameraComponent>("Camera", entity, [](C78E::CameraComponent& component) {
				auto& camera = component.camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.setProjectionType((C78E::SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.getProjectionType() == C78E::SceneCamera::ProjectionType::Perspective) {
					float perspectiveVerticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.setPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.getPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.setPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.getPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.setPerspectiveFarClip(perspectiveFar);
				}

				if (camera.getProjectionType() == C78E::SceneCamera::ProjectionType::Orthographic) {
					float orthoSize = camera.getOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.setOrthographicSize(orthoSize);

					float orthoNear = camera.getOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.setOrthographicNearClip(orthoNear);

					float orthoFar = camera.getOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.setOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.fixedAspectRatio);
				}
			});

			drawComponent<C78E::SpriteRendererComponent>("Sprite", entity, [this](C78E::SpriteRendererComponent& component) {
				ImGui::ColorEdit4("Color", &component.color[0], ImGuiColorEditFlags_NoInputs);
				drawAssetEdit(C78E::Asset::AssetType::Texture2D, component.texture, "Texture");
				ImGui::DragFloat("Tiling", &component.tilingFactor, .1f, .001f, 100.f);
			});
		}
		else {
			C78_EDITOR_ERROR("EntityManagerUI::drawEntityComponentList: called without m_SceneManager!");
		}
	}

	template<typename T>
	void EntityManagerUI::drawManagedAddComponentEntry(const std::string& entryName) {
		if (!m_ActiveEntity.hasComponent<T>()) {
			if (ImGui::MenuItem(entryName.c_str())) {
				m_ActiveEntity.addComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	template<typename T, typename UIFunction>
	void EntityManagerUI::drawComponent(const std::string& name, C78E::Entity entity, UIFunction uiFunction) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.hasComponent<T>()) {
			auto& component = entity.getComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings")) {
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open) {
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.removeComponent<T>();
		}
	}


	void EntityManagerUI::drawAssetPreview(C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label) {
		// show asset overwiew
		// edit -> call drawAssetEdit()
	}

	void EntityManagerUI::drawAssetEdit(C78E::Asset::AssetType type, C78E::AssetHandle& handle, const std::string& label) {
		auto sceneManager = m_SceneManager.lock();
		if (!sceneManager || !sceneManager->hasActiveProject()) return;
		auto assetManager = sceneManager->getProjectManager()->getActiveProject()->getEditorAssetManager();
		if (!sceneManager) return;

		if (!label.empty()) {
			ImGui::SeparatorText(label.c_str());
		} else ImGui::Separator();

		std::unordered_map<std::string, C78E::AssetHandle> assetofSameType{};
		int selected = 0;
		std::vector<const char*> comboEntries;

		for (auto& [entryHandle, entryMeta] : assetManager->getAssetRegistry())
			if(entryMeta.type == type) {
				if (entryHandle == handle)
					selected = static_cast<int>(comboEntries.size());
				assetofSameType[entryMeta.name] = entryHandle;
				comboEntries.push_back(entryMeta.name.c_str());
			}
		if (!assetofSameType.size()) {
			const char* none = "None";
			assetofSameType[none] = C78E::AssetHandle::invalid();
			comboEntries.push_back(none);
		}
		
		ImGui::Combo("##AssetEditSelect", &selected, comboEntries.data(), static_cast<int>(comboEntries.size()));

		handle = assetofSameType[comboEntries.at(selected)];

		ImGui::Text(("AssetHandle: " + C78E::AssetHandle::toString(handle)).c_str());
		ImGui::Text(("AssetType: " + C78E::Asset::assetTypeToString(type)).c_str());

		if (handle.isValid()) {

		}
		ImGui::Separator();
	}

}


