#pragma once

#include <C78E.h>

#define C78EDITOR_PROJECT_HISTORY_FILE "config/LatestProjects.yml"

namespace C78Editor {

	struct ProjectHistory {
	public:
		static inline std::set<C78E::FilePath> projects;

	public:
	private:
		static inline const C78E::FilePath configFile = C78EDITOR_PROJECT_HISTORY_FILE;

	public:
		static void load() {
			if (!std::filesystem::exists(configFile)) {
				C78E_INFO("Creating ProjectHistory! (First time setup)");
				save();
			}

			YAML::Node data;
			try {
				data = YAML::LoadFile(configFile.string());
			}
			catch (YAML::ParserException e) {
				C78E_ERROR("ProjectHistory::load: Loading ProjectHistory file: {} failed, due to: \n  {}", configFile, e.what());
				save(); return; // Using Defaults
			}

			auto node = data["ProjectHistory"];
			if (!node) { save(); return; }
			if(!node.IsSequence()) { save(); return; }
			std::set<C78E::FilePath> projectFiles = {};
			for (auto pathNode : node) {
				if(C78E::FileSystem::exists(pathNode.as<std::string>()) && C78E::FileSystem::extensionToEntryType(pathNode.as<std::string>()) == C78E::FileSystem::EntryType::Project)
					projectFiles.insert(pathNode.as<std::string>());
			}
			projects = projectFiles;
			C78E_INFO("ProjectHistory loaded!");
		}

		static void save() {
			YAML::Emitter out;
			{
				out << YAML::BeginMap; // Root
				out << YAML::Key << "ProjectHistory" << YAML::Value;
				{
					out << YAML::BeginSeq;// ProjectHistory
					for (auto& path : projects) {
						if(C78E::FileSystem::exists(path) && C78E::FileSystem::extensionToEntryType(path) == C78E::FileSystem::EntryType::Project)
							out << path.string();
					}
					out << YAML::EndSeq; // ProjectHistory
				}
				out << YAML::EndMap; // Root
			}

			std::ofstream fout(configFile);
			fout << out.c_str();
			C78E_INFO("ProjectHistory saved!");
		}


	
	};

}
