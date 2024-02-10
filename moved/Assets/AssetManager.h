#pragma once
#include "AssetLib.h"

#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "Model/Mesh.h"
#include "Material/Material.h"


namespace C78E {

	class AssetManager {
	public:
		static bool addShader	(std::string name, Ref<Shader> shader)		{ return s_ShaderLib.add	(name, shader); }
		static bool addTexture2D(std::string name, Ref<Texture2D> texture)	{ return s_Texture2DLib.add	(name, texture); }
		static bool addCubeMap	(std::string name, Ref<CubeMap> cubemap)	{ return s_CubeMapLib.add	(name, cubemap); }
		static bool addMesh		(std::string name, Ref<Mesh> mesh)			{ return s_MeshLib.add		(name, mesh); }
		static bool addMaterial	(std::string name, Ref<Material> material)	{ return s_MaterialLib.add	(name, material); }

		static void addShader	(std::string name, const std::string& filepath) { return s_ShaderLib.add	(name, filepath); }
		static void addTexture2D(std::string name, const std::string& filepath) { return s_Texture2DLib.add	(name, filepath); }
		static void addCubeMap	(std::string name, const std::string& filepath) { return s_CubeMapLib.add	(name, filepath); }
		static void addMesh		(std::string name, const std::string& filepath) { return s_MeshLib.add		(name, filepath); }
		static void addMaterial	(std::string name, const std::string& filepath) { return s_MaterialLib.add	(name, filepath); }

		static void loadShader		(std::string name) { return s_ShaderLib.load		(name); }
		static void loadTexture2D	(std::string name) { return s_Texture2DLib.load		(name); }
		static void loadCubeMap		(std::string name) { return s_CubeMapLib.load		(name); }
		static void loadMesh		(std::string name) { return s_MeshLib.load			(name); }
		static void loadMaterial	(std::string name) { return s_MaterialLib.load		(name); }

		static void loadShader		(std::string name, const std::string& filepath) { return s_ShaderLib.load	(name, filepath); }
		static void loadTexture2D	(std::string name, const std::string& filepath) { return s_Texture2DLib.load(name, filepath); }
		static void loadCubeMap		(std::string name, const std::string& filepath) { return s_CubeMapLib.load	(name, filepath); }
		static void loadMesh		(std::string name, const std::string& filepath) { return s_MeshLib.load		(name, filepath); }
		static void loadMaterial	(std::string name, const std::string& filepath) { return s_MaterialLib.load	(name, filepath); }

		static Ref<Shader>		getShader	(std::string name) { return s_ShaderLib.get(name); }
		static Ref<Texture2D>	getTexture2D(std::string name) { return s_Texture2DLib.get(name); }
		static Ref<CubeMap>		getCubeMap	(std::string name) { return s_CubeMapLib.get(name); }
		static Ref<Mesh>		getMesh		(std::string name) { return s_MeshLib.get(name); }
		static Ref<Material>	getMaterial	(std::string name) { return s_MaterialLib.get(name); }

		static Asset<Shader>	getShaderAsset		(std::string name) { return s_ShaderLib.get		(name); }
		static Asset<Texture2D>	getTexture2DAsset	(std::string name) { return s_Texture2DLib.get	(name); }
		static Asset<CubeMap>	getCubeMapAsset		(std::string name) { return s_CubeMapLib.get	(name); }
		static Asset<Mesh>		getMeshAsset		(std::string name) { return s_MeshLib.get		(name); }
		static Asset<Material>	getMaterialAsset	(std::string name) { return s_MaterialLib.get	(name); }

	private:
		static AssetLib<Shader>		AssetManager::s_ShaderLib;
		static AssetLib<Texture2D>	AssetManager::s_Texture2DLib;
		static AssetLib<CubeMap>	AssetManager::s_CubeMapLib;
		static AssetLib<Mesh>		AssetManager::s_MeshLib;
		static AssetLib<Material>	AssetManager::s_MaterialLib;

	};


}