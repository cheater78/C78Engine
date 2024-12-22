#pragma once
#include <C78E/Core/FileSystem/NativeFileExtensions.h>

#define C78E_VERSION_MAJOR 0
#define C78E_VERSION_MINOR 1
#define C78E_VERSION_PATCH 0
#define C78E_VERSION_NUMBER ( ((C78E_VERSION_MAJOR * 1000) + C78E_VERSION_MINOR) * 1000 + C78E_VERSION_PATCH )
#define C78E_VERSION_STRING ( "v" C78E_STRINGIFY_MACRO(C78E_VERSION_MAJOR) "." C78E_STRINGIFY_MACRO(C78E_VERSION_MINOR) "." C78E_STRINGIFY_MACRO(C78E_VERSION_PATCH))

// Default Names
#define C78E_ENGINE_TITLE "C78Engine"
#define C78E_WINDOW_DEFAULT_NAME "C78E Window"

#define C78E_DEFAULT_PROJECT_NAME "C78Project"
#define C78E_DEFAULT_ASSET_NAME "C78Asset"
#define C78E_DEFAULT_SCENE_NAME "C78Scene"

// Asset organization for Editor
  // relative to ProjectDirectory
#define C78E_ASSETLOCATION          "assets"
  // relative to AssetDirectory
#define C78E_ASSETLOCATION_REGISTRY "AssetRegistry" C78E_FILE_EXT_ASSETREGISTRY
#define C78E_ASSETLOCATION_SCRIPTMODULE "Scripts" C78E_FILE_EXT_BINARY
#define C78E_ASSETLOCATION_CACHE    "cache"
#define C78E_ASSETLOCATION_SCENE    "scenes"
#define C78E_ASSETLOCATION_MODEL    "models" 
#define C78E_ASSETLOCATION_TEXURE   "textures"
#define C78E_ASSETLOCATION_FONT     "fonts"
#define C78E_ASSETLOCATION_SHADER   "shaders"
#define C78E_ASSETLOCATION_SCRIPT   "scripts"


//

#ifdef C78E_PLATFORM_WINDOWS
#define C78E_PATH_MAX MAX_PATH
#elif defined C78E_PLATFORM_LINUX
#define C78E_PATH_MAX PATH_MAX
#endif // C78E_PLATFORM_WINDOWS

#define C78E_PROJECT_NAME_MAX_LENGTH 128


//C78Engine native HotKeys
#define C78E_HOTKEY_CONSOLE Input::KeyCombo{ { Input::Key::LeftAlt, Input::Key::F1 } }
//#define C78E_HOTKEY_SCREENSHOT Input::KeyCombo{ { Input::Key::LeftAlt, Input::Key::F2 } }
//#define C78E_HOTKEY_DEBUGOVERLAY Input::KeyCombo{ { Input::Key::LeftAlt, Input::Key::F3 } }

