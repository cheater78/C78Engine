#include <C78EPCH.h>
#include "SceneManager.h"

namespace C78E {

	/*
	* Constructor for SceneManager, assures a valid, needed, ProjectManager
	*/
	SceneManager::SceneManager(Ref<ProjectManager> projectManager)
		: m_ProjectManager(projectManager) {
		C78E_CORE_ASSERT(m_ProjectManager, "SceneManager::SceneManager: ProjectManager cannot be null!");
	}

	/*
	* Creates a new Scene, assigns the chosen name to the Meta, generates a Path for Serialization, sets it as ActiveScene and returns it
	*/
	Ref<Scene> SceneManager::createScene(const std::string& name, bool allowFileOverride) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::createScene: No active Project!")
		Ref<Scene> scene = C78E::createRef<Scene>();
		Ref<SceneImporter::SceneMeta> meta = C78E::createRef<SceneImporter::SceneMeta>();
		meta->name = name;
		meta->fileSource = FileSystem::getRelativePathTo(getNewAvailableSceneFile(scene, name, allowFileOverride), m_ProjectManager->getActiveProject()->getAssetDirectory());
		meta->type = C78E::Asset::Type::Scene;
		m_ActiveScene = m_ProjectManager->getEditorAssetManager()->addAsset(meta, scene);
		return m_ProjectManager->getEditorAssetManager()->getAssetAs<Scene>(m_ActiveScene);
	}

	/*
	* Saves an existing Scene
	*/
	bool SceneManager::saveScene(SceneHandle sceneHandle, const FilePath& sceneFile, bool overrideMetaSource) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::saveScene: No active Project!");
		Ref<EditorAssetManager> assetManager = m_ProjectManager->getEditorAssetManager();
		if (!sceneHandle)
			sceneHandle = m_ActiveScene;
		C78E_CORE_VALIDATE(assetManager->isValid(sceneHandle), return false, "SceneManager::saveScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		
		Ref<SceneImporter::SceneMeta> meta = assetManager->getMetaAs<SceneImporter::SceneMeta>(sceneHandle);
		C78E_CORE_VALIDATE(!(sceneFile.empty() && meta->fileSource.empty()), return false, "SceneManager::saveScene: Meta FileSource and sceneFile empty, atleast one must be set!")
		if(!sceneFile.empty() && overrideMetaSource)
			meta->fileSource = sceneFile;

		return SceneSerializer::exportScene(assetManager->getAssetAs<Scene>(sceneHandle), m_ProjectManager->getActiveProject()->getAssetDirectory() / FilePath((sceneFile.empty()) ? meta->fileSource : sceneFile));
	}

	/*
	* Deletes an existing Scene
	*/
	bool SceneManager::deleteScene(SceneHandle sceneHandle, bool fromDisk) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::deleteScene: No active Project!");
		C78E_CORE_VALIDATE(m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return false, "SceneManager::deleteScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		return m_ProjectManager->getEditorAssetManager()->removeAsset(sceneHandle, fromDisk);
	}

	/*
	* Provides an existing Scene
	*/
	Ref<Scene> SceneManager::getScene(SceneHandle sceneHandle) const {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getScene: No active Project!");
		C78E_CORE_VALIDATE(m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return nullptr, "SceneManager::getScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		return m_ProjectManager->getEditorAssetManager()->getAssetAs<Scene>(sceneHandle);
	}

	/*
	* Determines wheter this SceneManager has an active Scene
	*/
	bool SceneManager::hasActiveScene() const {
		return m_ProjectManager->hasActiveProject() && m_ActiveScene.isValid();
	}

	/*
	* Provides access to this SceneManager's active Scene
	*/
	Ref<Scene> SceneManager::getActiveScene() const {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getActiveScene: No active Project!");
		C78E_CORE_VALIDATE(hasActiveScene(), return nullptr, "SceneManager::getActiveScene: No active Scene!");
		return getScene(m_ActiveScene);
	}

	/*
	* Sets this SceneManager's active Scene by SceneHandle, NullHandle unsets the active Scene
	*/
	bool SceneManager::setActiveSceneHandle(SceneHandle sceneHandle) {
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return false, "SceneManager::getActiveScene: No active Project!");
		C78E_CORE_VALIDATE(!sceneHandle || m_ProjectManager->getEditorAssetManager()->isValid(sceneHandle), return false, "SceneManager::getActiveScene: SceneHandle is not associated with a Scene! SceneHandle: {}", sceneHandle);
		m_ActiveScene = sceneHandle;
		return true;
	}

	/*
	* Provides access to this SceneManager's active SceneHandel
	*/
	SceneHandle SceneManager::getActiveSceneHandle() const {
		return m_ActiveScene;
	}

	/*
	* Determines whether there is any Content in the active Scene, false if no active Scene
	*/
	bool SceneManager::activeSceneIsEmpty() const {
		return hasActiveScene() && getActiveScene()->isEmpty();
	}



	/*
	* Provides access to the underlying ProjectManager
	*/
	Ref<ProjectManager> SceneManager::getProjectManager() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return nullptr, "SceneManager::getProjectManager: ProjectManager is null!");
		return m_ProjectManager;
	}

	/*
	* Determines whether the underlying ProjectManager has an active Project
	*/
	bool SceneManager::hasActiveProject() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return false, "SceneManager::hasActiveProject: ProjectManager is null!");
		return m_ProjectManager->hasActiveProject();
	}

	/*
	* Provides access to the underlying ProjectManager's active Project
	*/
	Ref<Project> SceneManager::getActiveProject() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return nullptr, "SceneManager::getActiveProject: ProjectManager is null!");
		C78E_CORE_VALIDATE(m_ProjectManager->hasActiveProject(), return nullptr, "SceneManager::getActiveProject: ProjectManager does not have an active Project!");
		return m_ProjectManager->getActiveProject();
	}

	/*
	* Determines whether the underlying ProjectManager has an active Project File
	*/
	bool SceneManager::hasActiveProjectFile() const {
		C78E_CORE_VALIDATE(m_ProjectManager, return false, "SceneManager::hasActiveProjectFile: ProjectManager is null!");
		return m_ProjectManager->hasActiveProject() &&  m_ProjectManager->hasActiveProjectFile();
	}


	/*
	* Provides a Scene FilePath for saving Scenes
	*/
	FilePath SceneManager::getNewAvailableSceneFile(Ref<Scene> scene, const std::string& name, bool allowOverride) {
		const FilePath absoluteBase = m_ProjectManager->getActiveProject()->getAssetDirectory() / C78E_ASSETLOCATION_SCENE / name;
		FilePath newAvailableSceneFile = absoluteBase.string() + C78E_FILE_EXT_SCENE;
		for (uint64_t i = 2; !allowOverride && C78E::FileSystem::exists(newAvailableSceneFile); i++)
			newAvailableSceneFile = absoluteBase.string() + std::to_string(i) + C78E_FILE_EXT_SCENE;
		return newAvailableSceneFile;
	}

	/*
	* SceneImporter
	*/

	void SceneImporter::SceneMeta::serialize(EditorAssetManager::AssetDescriptor assetDescriptor, YAML::Emitter& appendTo) {
		// Scene can have all attributes inside its file, no need for specialization
		return;
	}

	void SceneImporter::SceneMeta::deserialize(const YAML::Node& serializedMeta, Ref<EditorAssetManager::AssetDescriptor>& appendTo) {
		// Scene can have all attributes inside its file, no need for specialization
		return;
	}

	SceneImporter::SceneImporter(const FilePath& assetDirectory)
		: EditorAssetManager::Importer(assetDirectory) {
	}

	SceneImporter::~SceneImporter() {
	}

	bool SceneImporter::compatible(const FilePath& assetFile) {
		return assetFile.has_extension() && assetFile.extension() == C78E_FILE_EXT_SCENE;
	}

	Ref<EditorAssetManager::ImportedAssetGroup> SceneImporter::import(Ref<EditorAssetManager::AssetDescriptorMap> importDescriptorMap) {
		C78E_CORE_VALIDATE(importDescriptorMap, return nullptr, "SceneImporter::import: importDescriptorMap was null!");
		FilePath sourceFile = verifyMapSourceFile(importDescriptorMap);
		C78E_CORE_VALIDATE(!sourceFile.empty(), return nullptr, "SceneImporter::import: verifyMapSourceFile failed!");
		Ref<EditorAssetManager::ImportedAssetGroup> importedAssets = createRef<EditorAssetManager::ImportedAssetGroup>();

		//importDescriptorMap should have size 1..
		Ref<Scene> scene = SceneSerializer::importScene(importDescriptorMap->begin()->first, m_AssetDirectory / sourceFile);

		EditorAssetManager::AssetLinkedMeta assetLinkedMeta{ scene, importDescriptorMap->begin()->second };
		importedAssets->insert(assetLinkedMeta);
		return importedAssets;
	}

	/*
	* SceneSerializer
	*/

	Ref<Scene> SceneSerializer::importScene(AssetHandle handle, const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return nullptr, "SceneManager::importScene: filepath provided was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return nullptr, "SceneManager::importScene: filepath was not an absolute path!");
		C78E_CORE_VALIDATE(FileSystem::isFile(filepath), return nullptr, "SceneManager::importScene: filepath provided is not a File!");
		C78E_CORE_VALIDATE(FileSystem::exists(filepath), return nullptr, "SceneManager::importScene: filepath provided does not exist!");

		FileSystem::EntryType fileType = FileSystem::extensionToEntryType(filepath);
		Ref<Scene> scene = createRef<Scene>();
		scene->handle() = handle;
		if (fileType == FileSystem::EntryType::Scene) {
			std::string serializedScene = FileSystem::readFileText(filepath);
			C78E_CORE_VALIDATE(!serializedScene.empty(), return nullptr, "SceneSerializer::importScene: Reading Text Scene failed!");
			C78E_CORE_VALIDATE(importEditorScene(scene, serializedScene), return nullptr, "SceneSerializer::importScene: Importing Text Scene failed!");
			return scene;
		}
		else if (fileType == FileSystem::EntryType::Binary) {
			Ref<ScopedBuffer> serializedScene = FileSystem::readFileBinary(filepath);
			C78E_CORE_VALIDATE(serializedScene, return nullptr, "SceneSerializer::importScene: Reading Binary Scene failed!");
			C78E_CORE_VALIDATE(importRuntimeScene(scene, serializedScene), return nullptr, "SceneSerializer::importScene:  Importing Binary Scene failed!");
			return scene;
		}
		C78E_CORE_ERROR("SceneSerializer::exportScene: filepath provided was neither a Scene nor Binary FileSystem::EntryType!");
		return nullptr;
	}

	bool SceneSerializer::exportScene(Ref<Scene> scene, const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return false, "SceneManager::exportScene: filepath provided was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return false, "SceneManager::exportScene: filepath was not an absolute path!");
		C78E_CORE_VALIDATE(FileSystem::isFile(filepath), return false, "SceneManager::exportScene: filepath provided is not a File!");
		C78E_CORE_VALIDATE(scene, return false, "SceneManager::exportScene: scene provided was nullptr!");

		FileSystem::EntryType fileType = FileSystem::extensionToEntryType(filepath);
		if (fileType == FileSystem::EntryType::Scene) {
			std::string serializedScene;
			C78E_CORE_VALIDATE(exportEditorScene(scene, serializedScene), return false, "SceneSerializer::exportScene: Exporting Scene to Text failed!");
			C78E_CORE_VALIDATE(FileSystem::writeFile(filepath, serializedScene), return false, "SceneSerializer::exportScene: Writing Exported Text Scene failed!");
			return true;
		}
		else if (fileType == FileSystem::EntryType::Binary) {
			Ref<ScopedBuffer> serializedScene;
			C78E_CORE_VALIDATE(exportRuntimeScene(scene, serializedScene), return false, "SceneSerializer::exportScene: Exporting Scene to Binary failed!");
			C78E_CORE_VALIDATE(FileSystem::writeFile(filepath, serializedScene), return false, "SceneSerializer::exportScene: Writing Exported Binary Scene failed!");
			return true;
		}
		C78E_CORE_ERROR("SceneSerializer::exportScene: filepath provided was neither a Scene nor Binary FileSystem::EntryType!");
		return false;
	}

	bool SceneSerializer::importEditorScene(Ref<Scene> scene, const std::string& serializedEditorScene) {
		YAML::Node node = YAML::Load(serializedEditorScene);

		C78E_CORE_VALIDATE(node, return false, "SceneManager::importEditorScene: serializedEditorScene could not be loaded!");

		if (YAML::Node nodeSceneEntities = node["Entities"]) {
			for (YAML::Node nodeEntity : nodeSceneEntities) {
				Entity entity;
				C78E_CORE_VALIDATE(importEditorEntity(scene, &entity, nodeEntity), continue, "SceneSerializer::importEditorScene: Importing Entity failed!");
				C78E_CORE_TRACE("SceneSerializer::importEditorScene: Entity imported: UUID: {}, Name: {}", entity.getUUID(), entity.getTag());
			}
		}
		return true;
	}

	bool SceneSerializer::importRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene) {
		return false;
	}

	bool SceneSerializer::exportEditorScene(Ref<Scene> scene, std::string& serializedEditorScene) {
		YAML::Emitter out;
		YAML::Emitter* emitter = &out;
		out << YAML::BeginMap;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->forEachEntity(
			[scene, emitter](auto entityID) -> void {
				Entity entity{ entityID, scene.get() };
				C78E_CORE_VALIDATE(exportEditorEntity(scene, &entity, *emitter), return, "SceneSerializer::importEditorScene: Exporting Entity failed!\n    UUID: {}\n    Name: {}", entity.getUUID(), entity.getTag());
				C78E_CORE_TRACE("SceneSerializer::importEditorScene: Entity exported: UUID: {}, Name: {}", entity.getUUID(), entity.getTag());
			}
		);
		out << YAML::EndSeq;
		out << YAML::EndMap;
		serializedEditorScene = std::string(out.c_str());
		return true;
	}

	bool SceneSerializer::exportRuntimeScene(Ref<Scene> scene, Ref<ScopedBuffer> serializedRuntimeScene) {
		return false;
	}


	bool SceneSerializer::importEditorEntity(Ref<Scene> scene, Entity* entity, const YAML::Node& serializedEditorEntity) {
		//UUID Component
		YAML::Node nodeUUID = serializedEditorEntity["Entity"];
		C78E_CORE_VALIDATE(nodeUUID, return false, "SceneManager::importEditorScene: Corrupted Entity! - Missing UUID!");

		//Tag Component
		YAML::Node nodeTagComp = serializedEditorEntity["TagComponent"];
		C78E_CORE_VALIDATE(nodeTagComp, return false, "SceneManager::importEditorScene: Corrupted Entity! - Missing TagComponent!");
		YAML::Node nodeTagCompTag = nodeTagComp["Tag"];
		C78E_CORE_VALIDATE(nodeTagCompTag, return false, "SceneManager::importEditorScene: Corrupted Entity! - Missing TagComponent.Tag!");

		//Transform Component
		YAML::Node nodeTransformComp = serializedEditorEntity["TransformComponent"];
		C78E_CORE_VALIDATE(nodeTransformComp, return false, "SceneManager::importEditorScene: Corrupted Entity! - Corrupted TransformComponent!");
		YAML::Node nodeTransformCompTranslation = nodeTransformComp["Translation"];
		YAML::Node nodeTransformCompRotation = nodeTransformComp["Rotation"];
		YAML::Node nodeTransformCompScale = nodeTransformComp["Scale"];
		C78E_CORE_VALIDATE(
			nodeTransformCompTranslation && nodeTransformCompRotation && nodeTransformCompScale,
			return false,
			"SceneManager::importEditorScene: Corrupted Entity! - Corrupted TransformComponent!\n    Translation:{}\n    Rotation:{}\n    Scale:{}",
			(nodeTransformCompTranslation) ? std::to_string(nodeTransformCompTranslation.as<glm::vec3>()) : "<corrupted>",
			(nodeTransformCompRotation) ? std::to_string(nodeTransformCompRotation.as<glm::vec3>()) : "<corrupted>",
			(nodeTransformCompScale) ? std::to_string(nodeTransformCompScale.as<glm::vec3>()) : "<corrupted>"
		);

		//Create Entity - since the mandatory Components were found, corrupted ones will be skipped
		*entity = scene->createEntityWithUUID(nodeUUID.as<UUID>(), nodeTagCompTag.as<std::string>());

		//Camera Component
		if (YAML::Node nodeCC = serializedEditorEntity["CameraComponent"]) {
			YAML::Node nodeCCCam = nodeCC["Camera"];
			YAML::Node nodeCCFAR = nodeCC["FixedAspectRatio"];
			if (nodeCCCam && nodeCCFAR) {
				YAML::Node nodeCCCamProjectionType = nodeCCCam["ProjectionType"];
				YAML::Node nodeCCCamPerspectiveFOV = nodeCCCam["PerspectiveFOV"];
				YAML::Node nodeCCCamPerspectiveNear = nodeCCCam["PerspectiveNear"];
				YAML::Node nodeCCCamPerspectiveFar = nodeCCCam["PerspectiveFar"];
				YAML::Node nodeCCCamOrthographicSize = nodeCCCam["OrthographicSize"];
				YAML::Node nodeCCCamOrthographicNear = nodeCCCam["OrthographicNear"];
				YAML::Node nodeCCCamOrthographicFar = nodeCCCam["OrthographicFar"];
				if (nodeCCCamProjectionType && nodeCCCamPerspectiveFOV && nodeCCCamPerspectiveNear && nodeCCCamPerspectiveFar && nodeCCCamOrthographicSize && nodeCCCamOrthographicNear && nodeCCCamOrthographicFar) {
					CameraComponent& cc = entity->addComponent<CameraComponent>();
					cc.camera.setProjectionType((SceneCamera::ProjectionType)nodeCCCamProjectionType.as<int>());
					cc.camera.setPerspectiveVerticalFOV(nodeCCCamPerspectiveFOV.as<float>());
					cc.camera.setPerspectiveNearClip(nodeCCCamPerspectiveNear.as<float>());
					cc.camera.setPerspectiveFarClip(nodeCCCamPerspectiveFar.as<float>());
					cc.camera.setOrthographicSize(nodeCCCamOrthographicSize.as<float>());
					cc.camera.setOrthographicNearClip(nodeCCCamOrthographicNear.as<float>());
					cc.camera.setOrthographicFarClip(nodeCCCamOrthographicFar.as<float>());
					cc.fixedAspectRatio = nodeCCFAR.as<bool>();
				}
				else {
					C78E_CORE_ERROR(
						"SceneManager::importEditorScene: Corrupted Entity! - Corrupted CameraComponent! - Corrupted Camera\n    ProjectionType:{}\n    PerspectiveFOV:{}\n    PerspectiveNear:{}\n    PerspectiveFar:{}\n    OrthographicSize:{}\n    OrthographicNear:{}\n    OrthographicFar:{}",
						(nodeCCCamProjectionType) ? ((nodeCCCamProjectionType.as<bool>()) ? "Orthographic" : "Perspective") : "<corrupted>",
						(nodeCCCamPerspectiveFOV) ? std::to_string(nodeCCCamPerspectiveFOV.as<float>()) : "<corrupted>",
						(nodeCCCamPerspectiveNear) ? std::to_string(nodeCCCamPerspectiveNear.as<float>()) : "<corrupted>",
						(nodeCCCamPerspectiveFar) ? std::to_string(nodeCCCamPerspectiveFar.as<float>()) : "<corrupted>",
						(nodeCCCamOrthographicSize) ? std::to_string(nodeCCCamOrthographicSize.as<float>()) : "<corrupted>",
						(nodeCCCamOrthographicNear) ? std::to_string(nodeCCCamOrthographicNear.as<float>()) : "<corrupted>",
						(nodeCCCamOrthographicFar) ? std::to_string(nodeCCCamOrthographicFar.as<float>()) : "<corrupted>"
					);
				}
			}
			else {
				C78E_CORE_ERROR(
					"SceneManager::importEditorScene: Corrupted Entity! - Corrupted TransformComponent!\n    Camera:{}\n    FixedAspectRatio:{}",
					(nodeCCCam) ? "<existing>" : "<corrupted>",
					(nodeCCFAR) ? std::to_string(nodeCCFAR.as<glm::vec3>()) : "<corrupted>"
				);
			}
		}

		//Model Component
		if (YAML::Node nodeMC = serializedEditorEntity["ModelComponent"]) {
			YAML::Node nodeModel = nodeMC["Model"];
			if (nodeModel) {
				entity->addComponent<ModelComponent>().model = nodeModel.as<UUID>();
			}
			else {
				C78E_CORE_ERROR("SceneManager::importEditorScene: Corrupted Entity! - Corrupted ModelComponent.Model!");
			}
		}

		//SpriteRenderer Component
		if (YAML::Node nodeSP = serializedEditorEntity["SpriteRendererComponent"]) {
			YAML::Node nodeSPColor = nodeSP["Color"];
			YAML::Node nodeSPTexture = nodeSP["Texture"];
			YAML::Node nodeSPTiling = nodeSP["Tiling"];
			if (nodeSPColor && nodeSPTexture && nodeSPTiling) {
				SpriteRendererComponent& src = entity->addComponent<SpriteRendererComponent>();
				src.color = nodeSPColor.as<glm::vec4>();
				src.texture = nodeSPTexture.as<AssetHandle>();
				src.tilingFactor = nodeSPTiling.as<float>();
			}
			else {
				C78E_CORE_ERROR(
					"SceneManager::importEditorScene: Corrupted Entity! - Corrupted SpriteRendererComponent!\n    Color:{}\n    Texture:{}\n    Tiling:{}",
					(nodeSPColor) ? std::to_string(nodeSPColor.as<glm::vec3>()) : "<corrupted>",
					(nodeSPTexture) ? std::to_string(nodeSPTexture.as<glm::vec3>()) : "<corrupted>",
					(nodeSPTiling) ? std::to_string(nodeSPTiling.as<glm::vec3>()) : "<corrupted>"
				);
			}
		}

		//Text Component
		if (YAML::Node nodeTC = serializedEditorEntity["TextComponent"]) {
			YAML::Node nodeTCDisplayText = nodeTC["DisplayText"];
			YAML::Node nodeTCColor = nodeTC["Color"];
			YAML::Node nodeTCFont = nodeTC["Font"];
			YAML::Node nodeTCKerning = nodeTC["Kerning"];
			YAML::Node nodeTCLineSpacing = nodeTC["LineSpacing"];

			if (nodeTCDisplayText && nodeTCColor && nodeTCFont && nodeTCKerning && nodeTCLineSpacing) {
				TextComponent& tc = entity->addComponent<TextComponent>();
				tc.textString = nodeTCDisplayText.as<std::string>();
				tc.color = nodeTCColor.as<glm::vec4>();
				tc.fontAsset = nodeTCFont.as<AssetHandle>();
				tc.kerning = nodeTCKerning.as<float>();
				tc.lineSpacing = nodeTCLineSpacing.as<float>();
			}
			else {
				C78E_CORE_ERROR(
					"SceneManager::importEditorScene: Corrupted Entity! - Corrupted TextComponent!\n    DisplayText:{}\n    Color:{}\n    Font:{}\n    Kerning:{}\n    LineSpacing:{}",
					(nodeTCDisplayText) ? nodeTCDisplayText.as<std::string>() : "<corrupted>",
					(nodeTCColor) ? std::to_string(nodeTCColor.as<glm::vec4>()) : "<corrupted>",
					(nodeTCFont) ? std::to_string(nodeTCFont.as<AssetHandle>()) : "<corrupted>",
					(nodeTCKerning) ? std::to_string(nodeTCKerning.as<float>()) : "<corrupted>",
					(nodeTCLineSpacing) ? std::to_string(nodeTCLineSpacing.as<float>()) : "<corrupted>"
				);
			}
		}

		//SkyBox Component
		if (YAML::Node skyboxComponent = serializedEditorEntity["SkyBoxComponent"]) {
			auto& src = entity->addComponent<SkyBoxComponent>();
			src.skybox = skyboxComponent["CubeMap"].as<AssetHandle>();
		}
		return true;
	}

	bool SceneSerializer::importRuntimeEntity(Ref<Scene> scene, Entity* entity, Ref<ScopedBuffer> serializedRuntimeEnity) {
		return false;
	}

	bool SceneSerializer::exportEditorEntity(Ref<Scene> scene, Entity* entity, YAML::Emitter& serializedEditorEntity) {
		C78E_CORE_VALIDATE(entity, return false, "SceneSerializer::exportEditorEntity: Corrupted Entity! Null Entity!");
		C78E_CORE_VALIDATE(entity->hasComponent<IDComponent>(), return false, "SceneSerializer::exportEditorEntity: Corrupted Entity! Missing ID Component!");
		C78E_CORE_VALIDATE(entity->hasComponent<TagComponent>(), return false, "SceneSerializer::exportEditorEntity: Corrupted Entity! Missing Tag Component!");
		C78E_CORE_VALIDATE(entity->hasComponent<TransformComponent>(), return false, "SceneSerializer::exportEditorEntity: Corrupted Entity! Missing Transform Component!");

		serializedEditorEntity << YAML::BeginMap; // Entity

		{ //UUID
			serializedEditorEntity << YAML::Key << "Entity" << YAML::Value << entity->getUUID();
		}

		{ //TagComponent
			serializedEditorEntity << YAML::Key << "TagComponent";
			serializedEditorEntity << YAML::BeginMap; // TagComponent
			serializedEditorEntity << YAML::Key << "Tag" << YAML::Value << entity->getComponent<TagComponent>().tag;
			serializedEditorEntity << YAML::EndMap; // TagComponent
		}

		{ //TransformComponent
			serializedEditorEntity << YAML::Key << "TransformComponent";
			serializedEditorEntity << YAML::BeginMap; // TransformComponent
			auto& tc = entity->getComponent<TransformComponent>();
			serializedEditorEntity << YAML::Key << "Translation" << YAML::Value << tc.translation;
			serializedEditorEntity << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			serializedEditorEntity << YAML::Key << "Scale" << YAML::Value << tc.scale;
			serializedEditorEntity << YAML::EndMap; // TransformComponent
		}

		if (entity->hasComponent<CameraComponent>()) {
			serializedEditorEntity << YAML::Key << "CameraComponent";
			serializedEditorEntity << YAML::BeginMap; // CameraComponent
			auto& cameraComponent = entity->getComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;
			serializedEditorEntity << YAML::Key << "Camera" << YAML::Value;
			serializedEditorEntity << YAML::BeginMap; // Camera
			serializedEditorEntity << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.getProjectionType();
			serializedEditorEntity << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
			serializedEditorEntity << YAML::Key << "PerspectiveNear" << YAML::Value << camera.getPerspectiveNearClip();
			serializedEditorEntity << YAML::Key << "PerspectiveFar" << YAML::Value << camera.getPerspectiveFarClip();
			serializedEditorEntity << YAML::Key << "OrthographicSize" << YAML::Value << camera.getOrthographicSize();
			serializedEditorEntity << YAML::Key << "OrthographicNear" << YAML::Value << camera.getOrthographicNearClip();
			serializedEditorEntity << YAML::Key << "OrthographicFar" << YAML::Value << camera.getOrthographicFarClip();
			serializedEditorEntity << YAML::EndMap; // Camera
			serializedEditorEntity << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;
			serializedEditorEntity << YAML::EndMap; // CameraComponent
		}

		if (entity->hasComponent<ModelComponent>()) {
			serializedEditorEntity << YAML::Key << "ModelComponent";
			serializedEditorEntity << YAML::BeginMap; // ModelComponent
			auto& modelComponent = entity->getComponent<ModelComponent>();
			auto& model = modelComponent.model;
			serializedEditorEntity << YAML::Key << "Model" << YAML::Value << model;
			serializedEditorEntity << YAML::EndMap; // ModelComponent
		}

		if (entity->hasComponent<SpriteRendererComponent>()) {
			serializedEditorEntity << YAML::Key << "SpriteRendererComponent";
			serializedEditorEntity << YAML::BeginMap; // SpriteRendererComponent
			auto& spriteComponent = entity->getComponent<SpriteRendererComponent>();
			serializedEditorEntity << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
			serializedEditorEntity << YAML::Key << "Texture" << YAML::Value << spriteComponent.texture;
			serializedEditorEntity << YAML::Key << "Tiling" << YAML::Value << spriteComponent.tilingFactor;
			serializedEditorEntity << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity->hasComponent<TextComponent>()) {
			serializedEditorEntity << YAML::Key << "TextComponent";
			serializedEditorEntity << YAML::BeginMap; // TextComponent
			auto& spriteComponent = entity->getComponent<TextComponent>();
			serializedEditorEntity << YAML::Key << "DisplayText" << YAML::Value << spriteComponent.textString;
			serializedEditorEntity << YAML::Key << "Color" << YAML::Value << spriteComponent.color;
			serializedEditorEntity << YAML::Key << "Font" << YAML::Value << spriteComponent.fontAsset;
			serializedEditorEntity << YAML::Key << "Kerning" << YAML::Value << spriteComponent.kerning;
			serializedEditorEntity << YAML::Key << "LineSpacing" << YAML::Value << spriteComponent.lineSpacing;
			serializedEditorEntity << YAML::EndMap; // TextComponent
		}

		if (entity->hasComponent<SkyBoxComponent>()) {
			serializedEditorEntity << YAML::Key << "SkyBoxComponent";
			serializedEditorEntity << YAML::BeginMap; // SkyBoxComponent
			auto& skyblockComponent = entity->getComponent<SkyBoxComponent>();
			serializedEditorEntity << YAML::Key << "CubeMap" << YAML::Value << skyblockComponent.skybox;
			serializedEditorEntity << YAML::EndMap; // SkyBoxComponent
		}

		serializedEditorEntity << YAML::EndMap; // Entity
		return true;
	}

	bool SceneSerializer::exportRuntimeEntity(Ref<Scene> scene, Entity* entity, Ref<ScopedBuffer> serializedRuntimeEnity) {
		C78E_CORE_VALIDATE(entity, return false, "SceneSerializer::exportRuntimeEntity: Corrupted Entity! Null Entity!");
		C78E_CORE_VALIDATE(entity->hasComponent<IDComponent>(), return false, "SceneSerializer::exportRuntimeEntity: Corrupted Entity! Missing ID Component!");
		C78E_CORE_VALIDATE(entity->hasComponent<TagComponent>(), return false, "SceneSerializer::exportRuntimeEntity: Corrupted Entity! Missing Tag Component!");
		C78E_CORE_VALIDATE(entity->hasComponent<TransformComponent>(), return false, "SceneSerializer::exportRuntimeEntity: Corrupted Entity! Missing Transform Component!");

		C78E_CORE_ASSERT(false, "Impl me!");

		return true;
	}

}