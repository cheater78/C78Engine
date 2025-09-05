#pragma once
// C78Engine native file extensions

#define C78E_FILE_EXT_SCENE ".sce" // File extension for serialized Scenes (at EditorTime)
#define C78E_FILE_EXT_PROJECT ".pce" // File extension for serialized Projects (at EditorTime)
#define C78E_FILE_EXT_ASSETREGISTRY ".ace" // File extension for serialized AssetRegistries (at EditorTime)

#define C78E_FILE_EXT_BINARY ".bce" // File extension for all binary files
#define C78E_FILE_EXT_ASSETPACK C78E_FILE_EXT_BINARY // File extension for (binary) serialized Assets (at RunTime)
#define C78E_FILE_EXT_SHADERPACK C78E_FILE_EXT_BINARY // File extension for (binary) serialized Shaders (at RunTime)

#define C78E_FILE_EXT_CACHE ".cce" // File extension for caching

// C78 Engine Naming Pattern
// .ace - 
// .bce - 
// .cce
// .dce
// .ece
// .fce
// .gce
// .hce
// .ice
// .jce
// .kce
// .lce
// .mce
// .nce
// .oce
// .pce - 
// .qce
// .rce
// .sce - 
// .tce
// .uce
// .vce
// .wce
// .xce
// .yce
// .zce


// Shader Files - Name Fragments
#define C78E_FILE_EXT_FRAG_SHADER_VERTEX ".vert" 
#define C78E_FILE_EXT_FRAG_SHADER_TESSELATION ".tess"
#define C78E_FILE_EXT_FRAG_SHADER_TESSEVALUATION ".tese"
#define C78E_FILE_EXT_FRAG_SHADER_GEOMETRY ".geom"
#define C78E_FILE_EXT_FRAG_SHADER_FRAGMENT ".frag"
#define C78E_FILE_EXT_FRAG_SHADER_COMPUTE ".comp"

#define C78E_FILE_EXT_SHADER_GLSL_SOURCE ".glsl"
#define C78E_FILE_EXT_SHADER_HLSL_SOURCE ".hlsl"

#define C78E_FILE_EXT_SHADER_SPIRV_BINARY ".spv"
