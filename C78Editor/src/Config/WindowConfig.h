#pragma once
#include <C78E.h>
#include <C78Elibs.h>

#define C78EDITOR_CONFIG_FILE "config/editor.yml"

namespace C78Editor {

	class WindowConfig {
	public:
		static inline glm::vec<2, uint32_t> s_DefaultWindowSize = { 1920, 1080 };
		static inline glm::vec<2, uint32_t> s_LastWindowSize = { 1920, 1080 };
	private:
		static inline const C78E::FilePath configFile = C78EDITOR_CONFIG_FILE;

	public:
		static void load() {
			if (!std::filesystem::exists(configFile)) {
				C78_EDITOR_INFO("Creating WindowConfig! (First time setup)");
				save();
			}

			YAML::Node data;
			try {
				data = YAML::LoadFile(configFile.string());
			} catch (YAML::ParserException e) {
				C78_EDITOR_ERROR("WindowConfig::load: Loading WindowConfig file: {} failed, due to: \n  {}", configFile, e.what());
				save(); return; // Using Defaults
			}

			auto node = data["WindowConfig"];
			if (!node) { save(); return; }
			s_DefaultWindowSize = node["DefaultWindowSize"].as<glm::vec<2, uint32_t>>();
			s_LastWindowSize = node["LastWindowSize"].as<glm::vec<2, uint32_t>>();

			C78_EDITOR_INFO("WindowConfig loaded!");
		}

		static void save() {
			YAML::Emitter out;
			{
				out << YAML::BeginMap; // Root
				out << YAML::Key << "WindowConfig" << YAML::Value;
				{
					out << YAML::BeginMap;// WindowConfig
					out << YAML::Key << "DefaultWindowSize" << YAML::Value << s_DefaultWindowSize;
					out << YAML::Key << "LastWindowSize" << YAML::Value << s_LastWindowSize;
					out << YAML::EndMap; // WindowConfig
				}
				out << YAML::EndMap; // Root
			}

			std::ofstream fout(configFile);
			fout << out.c_str();
			C78_EDITOR_INFO("WindowConfig saved!");
		}

	};


}