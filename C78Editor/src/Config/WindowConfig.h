#pragma once
#include <C78E.h>
#include <C78Elibs.h>


namespace C78Editor {

	class WindowConfig {
	private:
		static const C78E::FilePath configFile;

	public:

		static void load() {
			if (!std::filesystem::exists(configFile)) save();

			YAML::Node data;
			try {
				data = YAML::LoadFile(configFile.string());
			} catch (YAML::ParserException e) {
				C78_EDITOR_ERROR("WindowConfig::load: Loading WindowConfig file: {} failed, due to: \n  {}", configFile, e.what());
				save(); return; // Using Defaults
			}

			auto node = data["WindowConfig"];
			if (!node) { save(); return; }
			s_StartWindowSize = node["StartWindowSize"].as<glm::vec2>();
			s_EditorWindowSize = node["EditorWindowSize"].as<glm::vec2>();
		}

		static void save() {
			YAML::Emitter out;
			{
				out << YAML::BeginMap; // Root
				out << YAML::Key << "WindowConfig" << YAML::Value;
				{
					out << YAML::BeginMap;// WindowConfig
					out << YAML::Key << "StartWindowSize" << YAML::Value << s_StartWindowSize;
					out << YAML::Key << "EditorWindowSize" << YAML::Value << s_EditorWindowSize;
					out << YAML::EndMap; // WindowConfig
				}
				out << YAML::EndMap; // Root
			}

			std::ofstream fout(configFile);
			fout << out.c_str();
		}

		static glm::vec2 getStartWindowSize() {
			C78E::System::Monitor monitor = C78E::System::getPrimaryMonitor();
			return { monitor.width * s_StartWindowSize.x, monitor.height * s_StartWindowSize.y };
		}

		static glm::vec2 getEditorWindowSize() {
			C78E::System::Monitor monitor = C78E::System::getPrimaryMonitor();
			return { monitor.width * s_EditorWindowSize.x, monitor.height * s_EditorWindowSize.y };
		}

	private:
		static glm::vec2 s_StartWindowSize;
		static glm::vec2 s_EditorWindowSize;
	};


}