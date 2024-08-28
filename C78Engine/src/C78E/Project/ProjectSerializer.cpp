#include "C78ePCH.h"
#include "ProjectSerializer.h"

#include <C78E/Utils/Yaml/YamlUtils.h>

namespace C78E {

	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		: m_Project(project)
	{ }

	bool ProjectSerializer::serializeProject(const FilePath& filepath) {
		const auto& config = m_Project->getConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap;// Project
				out << YAML::Key << "Name" << YAML::Value << config.name;
				out << YAML::Key << "StartScene" << YAML::Value << (uint64_t)config.startScene;
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.assetDirectory.string();
				out << YAML::Key << "AssetRegistryPath" << YAML::Value << config.assetRegistryPath.string();
				out << YAML::Key << "ScriptModulePath" << YAML::Value << config.scriptModulePath.string();
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::deserializeProject(const FilePath& filepath) {
		auto& config = m_Project->getConfig();

		YAML::Node data;
		try {
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e) {
			C78_CORE_ERROR("Failed to load project file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
			return false;

		config.name = projectNode["Name"].as<std::string>();
		config.startScene = projectNode["StartScene"].as<uint64_t>();
		config.assetDirectory = projectNode["AssetDirectory"].as<std::string>();
		if (projectNode["AssetRegistryPath"])
			config.assetRegistryPath = projectNode["AssetRegistryPath"].as<std::string>();
		config.scriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
		return true;
	}

}
