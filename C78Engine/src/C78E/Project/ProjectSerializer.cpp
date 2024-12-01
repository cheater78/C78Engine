#include "C78EPCH.h"
#include "ProjectSerializer.h"

#include <C78E/Utils/Yaml/YamlUtils.h>

namespace C78E {

	Ref<Project> ProjectSerializer::importProject(const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return nullptr, "ProjectSerializer::importProject: FilePath provided was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return nullptr, "ProjectSerializer::importProject: FilePath provided was not an absolute path!");
		C78E_CORE_VALIDATE(FileSystem::isFile(filepath), return nullptr, "ProjectSerializer::importProject: FilePath provided is not a File!");
		C78E_CORE_VALIDATE(FileSystem::exists(filepath), return nullptr, "ProjectSerializer::importProject: FilePath provided does not exist!");
		
		FileSystem::EntryType fileType = FileSystem::extensionToEntryType(filepath);
		Ref<Project> project = createRef<Project>();
		if(fileType == FileSystem::EntryType::Project) {
			std::string serializedProject = FileSystem::readFileText(filepath);
			C78E_CORE_VALIDATE(!serializedProject.empty(), return nullptr, "ProjectSerializer::importProject: Reading Text Project failed!");
			C78E_CORE_VALIDATE(importEditorProject(project, serializedProject), return nullptr, "ProjectSerializer::importProject: Importing Text Project failed!");
			project->getConfig().normalize(filepath.parent_path()); // maybe?
			return project;
		} else if (fileType == FileSystem::EntryType::Binary) {
			Ref<ScopedBuffer> serializedProject = FileSystem::readFileBinary(filepath);
			C78E_CORE_VALIDATE(serializedProject, return nullptr, "ProjectSerializer::importProject: Reading Binary Project failed!");
			C78E_CORE_VALIDATE(importRuntimeProject(project, serializedProject), return nullptr, "ProjectSerializer::importProject:  Importing Binary Project failed!");
			return project;
		}
		C78E_CORE_ERROR("ProjectSerializer::importProject: FilePath provided was neither a Project nor Binary FileSystem::EntryType!");
		return nullptr;
	}

	bool ProjectSerializer::exportProject(Ref<Project> project, const FilePath& filepath) {
		C78E_CORE_VALIDATE(!filepath.empty(), return false, "ProjectSerializer::exportProject: FilePath provided was empty!");
		C78E_CORE_VALIDATE(filepath.is_absolute(), return false, "ProjectSerializer::exportProject: FilePath provided was not an absolute path! File: {}", filepath);
		C78E_CORE_VALIDATE(FileSystem::isFile(filepath), return false, "ProjectSerializer::exportProject: FilePath provided is not a File! File: {}", filepath);
		C78E_CORE_VALIDATE(project, return false, "ProjectSerializer::exportProject: Project provided was nullptr!");

		FileSystem::EntryType fileType = FileSystem::extensionToEntryType(filepath);
		if(fileType == FileSystem::EntryType::Project) {
			std::string serializedProject;
			C78E_CORE_VALIDATE(exportEditorProject(project, serializedProject), return false, "ProjectSerializer::exportProject: Exporting Project to Text failed!");
			C78E_CORE_VALIDATE(FileSystem::writeFile(filepath, serializedProject), return false, "ProjectSerializer::exportProject: Writing Exported Text Project failed!");
			return true;
		} else if (fileType == FileSystem::EntryType::Binary) {
			Ref<ScopedBuffer> serializedProject;
			C78E_CORE_VALIDATE(exportRuntimeProject(project, serializedProject), return false, "ProjectSerializer::exportProject: Exporting Project to Binary failed!");
			C78E_CORE_VALIDATE(FileSystem::writeFile(filepath, serializedProject), return false, "ProjectSerializer::exportProject: Writing Exported Binary Project failed!");
			return true;
		}
		C78E_CORE_ERROR("ProjectSerializer::exportProject: FilePath provided was neither a Project nor Binary FileSystem::EntryType!");
		return false;
	}

	bool ProjectSerializer::importEditorProject(Ref<Project> project, const std::string& serializedEditorProject) {
		Project::Config& config = project->getConfig();

		YAML::Node node = YAML::Load(serializedEditorProject);
		C78E_CORE_VALIDATE(node, return false, "ProjectSerializer::importEditorProject: serializedEditorProject could not be loaded!");
		YAML::Node nodeProject = node["Project"];
		C78E_CORE_VALIDATE(nodeProject, return false, "ProjectSerializer::deserializeProject: File does not contain a Project!");

		YAML::Node nodeProjectName = nodeProject["Name"];
		C78E_CORE_VALIDATE(nodeProjectName, return false, "ProjectSerializer::deserializeProject: Project File does not contain a Name!");
		config.name = nodeProjectName.as<std::string>();

		YAML::Node nodeProjectStartScene = nodeProject["StartScene"];
		C78E_CORE_VALIDATE(nodeProjectStartScene, return false, "ProjectSerializer::deserializeProject: Project File does not contain a StartScene!");
		config.startScene = nodeProjectStartScene.as<UUID>();
		
		YAML::Node nodeProjectAssetDirectory = nodeProject["AssetDirectory"];
		C78E_CORE_VALIDATE(nodeProjectAssetDirectory, return false, "ProjectSerializer::deserializeProject: Project File does not contain a AssetDirectory!");
		config.assetDirectory = nodeProjectAssetDirectory.as<std::string>();
		
		YAML::Node nodeProjectAssetRegistryPath = nodeProject["AssetRegistry"];
		C78E_CORE_VALIDATE(nodeProjectAssetRegistryPath, return false, "ProjectSerializer::deserializeProject: Project File does not contain a AssetRegistry!");
		config.assetRegistryPath = nodeProjectAssetRegistryPath.as<std::string>();

		YAML::Node nodeProjectScriptModulePath = nodeProject["ScriptModule"];
		C78E_CORE_VALIDATE(nodeProjectScriptModulePath, return false, "ProjectSerializer::deserializeProject: Project File does not contain a ScriptModule!");
		config.scriptModulePath = nodeProjectScriptModulePath.as<std::string>();

		return true;
	}

	bool ProjectSerializer::importRuntimeProject(Ref<Project> project, Ref<ScopedBuffer> serializedRuntimeProject) {
		return false;
	}

	bool ProjectSerializer::exportEditorProject(Ref<Project> project, std::string& serializedEditorProject) {
		C78E_CORE_VALIDATE(project, return false, "ProjectSerializer::exportEditorProject: Project provided was nullptr!");
		const Project::Config& config = project->getConfig();
		//TODO: validate config
		YAML::Emitter out;
		out << YAML::BeginMap; // Root
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap;// Project
			out << YAML::Key << "Name" << YAML::Value << config.name;
			out << YAML::Key << "StartScene" << YAML::Value << config.startScene;
			out << YAML::Key << "AssetDirectory" << YAML::Value << config.assetDirectory.string();
			out << YAML::Key << "AssetRegistry" << YAML::Value << config.assetRegistryPath.string();
			out << YAML::Key << "ScriptModule" << YAML::Value << config.scriptModulePath.string();
			out << YAML::EndMap; // Project
		}
		out << YAML::EndMap; // Root
		serializedEditorProject = std::string(out.c_str());
		return true;
	}

	bool ProjectSerializer::exportRuntimeProject(Ref<Project> project, Ref<ScopedBuffer> serializedRuntimeProject) {
		return false;
	}
}
