#include "WindowConfig.h"

namespace C78Editor {

	const C78E::FilePath WindowConfig::configFile = "./config/editorWindow.cfg";

	glm::vec2 WindowConfig::s_StartWindowSize = { 0.3, 0.3 };
	glm::vec2 WindowConfig::s_EditorWindowSize = { 0.625, 0.625 };

}
