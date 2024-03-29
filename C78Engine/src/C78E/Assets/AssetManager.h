#pragma once
#include "AssetLib.h"

#include <C78E/Assets/Model/Model.h>
#include <C78E/Assets/Mesh/Mesh.h>
#include <C78E/Assets/Material/Material.h>
#include <C78E/Assets/Shader/Shader.h>
#include <C78E/Assets/Texture/Texture.h>


namespace C78E {

	class AssetManager {
	public:
		static bool addShader	(std::string name, Ref<Shader> shader, std::string source = NO_ASSET_SOURCE)		{ return s_ShaderLib.add	(name, shader, source); }
		static bool addTexture2D(std::string name, Ref<Texture2D> texture, std::string source = NO_ASSET_SOURCE)	{ return s_Texture2DLib.add	(name, texture, source); }
		static bool addCubeMap	(std::string name, Ref<CubeMap> cubemap, std::string source = NO_ASSET_SOURCE)		{ return s_CubeMapLib.add	(name, cubemap, source); }
		static bool addMesh		(std::string name, Ref<Mesh> mesh, std::string source = NO_ASSET_SOURCE)			{ return s_MeshLib.add		(name, mesh, source); }
		static bool addMaterial	(std::string name, Ref<Material> material, std::string source = NO_ASSET_SOURCE)	{ return s_MaterialLib.add	(name, material, source); }
		static bool addModel	(std::string name, Ref<Model> material, std::string source = NO_ASSET_SOURCE)		{ return s_ModelLib.add		(name, material, source); }

		static bool regShader	(std::string name, const std::string& filepath) { return s_ShaderLib.reg	(name, filepath); }
		static bool regTexture2D(std::string name, const std::string& filepath) { return s_Texture2DLib.reg	(name, filepath); }
		static bool regCubeMap	(std::string name, const std::string& filepath) { return s_CubeMapLib.reg	(name, filepath); }
		static bool regMesh		(std::string name, const std::string& filepath) { return s_MeshLib.reg		(name, filepath); }
		static bool regMaterial	(std::string name, const std::string& filepath) { return s_MaterialLib.reg	(name, filepath); }
		static bool regModel	(std::string name, const std::string& filepath) { return s_ModelLib.reg		(name, filepath); }

		static bool remShader	(std::string name) { return s_ShaderLib.remove		(name); }
		static bool remTexture2D(std::string name) { return s_Texture2DLib.remove	(name); }
		static bool remCubeMap	(std::string name) { return s_CubeMapLib.remove		(name); }
		static bool remMesh		(std::string name) { return s_MeshLib.remove		(name); }
		static bool remMaterial	(std::string name) { return s_MaterialLib.remove	(name); }
		static bool remModel	(std::string name) { return s_ModelLib.remove		(name); }

		static void loadShader		(std::string name) { return s_ShaderLib.load		(name); }
		static void loadTexture2D	(std::string name) { return s_Texture2DLib.load		(name); }
		static void loadCubeMap		(std::string name) { return s_CubeMapLib.load		(name); }
		static void loadMesh		(std::string name) { return s_MeshLib.load			(name); }
		static void loadMaterial	(std::string name) { return s_MaterialLib.load		(name); }
		static void loadModel		(std::string name) { return s_ModelLib.load			(name); }

		static void loadShader		(std::string name, const std::string& filepath) { return s_ShaderLib.load	(name, filepath); }
		static void loadTexture2D	(std::string name, const std::string& filepath) { return s_Texture2DLib.load(name, filepath); }
		static void loadCubeMap		(std::string name, const std::string& filepath) { return s_CubeMapLib.load	(name, filepath); }
		static void loadMesh		(std::string name, const std::string& filepath) { return s_MeshLib.load		(name, filepath); }
		static void loadMaterial	(std::string name, const std::string& filepath) { return s_MaterialLib.load	(name, filepath); }
		static void loadModel		(std::string name, const std::string& filepath) { return s_ModelLib.load	(name, filepath); }

		static Ref<Shader>		getShader	(std::string name) { return s_ShaderLib.get(name); }
		static Ref<Texture2D>	getTexture2D(std::string name) { return s_Texture2DLib.get(name); }
		static Ref<CubeMap>		getCubeMap	(std::string name) { return s_CubeMapLib.get(name); }
		static Ref<Mesh>		getMesh		(std::string name) { return s_MeshLib.get(name); }
		static Ref<Material>	getMaterial	(std::string name) { return s_MaterialLib.get(name); }
		static Ref<Model>		getModel	(std::string name) { return s_ModelLib.get(name); }

		static Asset<Shader>	getShaderAsset		(std::string name) { return s_ShaderLib.get		(name); }
		static Asset<Texture2D>	getTexture2DAsset	(std::string name) { return s_Texture2DLib.get	(name); }
		static Asset<CubeMap>	getCubeMapAsset		(std::string name) { return s_CubeMapLib.get	(name); }
		static Asset<Mesh>		getMeshAsset		(std::string name) { return s_MeshLib.get		(name); }
		static Asset<Material>	getMaterialAsset	(std::string name) { return s_MaterialLib.get	(name); }
		static Asset <Model>	getModelAsset		(std::string name) { return s_ModelLib.get		(name); }

		static std::set<std::string> getAllShader() { return s_ShaderLib.getAllNames(); }
		static std::set<std::string> getAllTexture2D() { return s_Texture2DLib.getAllNames(); }
		static std::set<std::string> getAllCubeMap() { return s_CubeMapLib.getAllNames(); }
		static std::set<std::string> getAllMesh() { return s_MeshLib.getAllNames(); }
		static std::set<std::string> getAllMaterial() { return s_MaterialLib.getAllNames(); }
		static std::set<std::string> getAllModel() { return s_ModelLib.getAllNames(); }

		static bool renameShader(std::string name, std::string newName) { s_ShaderLib.rename(name, newName); }
		static bool renameTexture2D(std::string name, std::string newName) { s_Texture2DLib.rename(name, newName); }
		static bool renameCubeMap(std::string name, std::string newName) { s_CubeMapLib.rename(name, newName); }
		static bool renameMesh(std::string name, std::string newName) { s_MeshLib.rename(name, newName); }
		static bool renameMaterial(std::string name, std::string newName) { s_MaterialLib.rename(name, newName); }
		static bool renameModel(std::string name, std::string newName) { s_ModelLib.rename(name, newName); }

	private:
		static AssetLib<Shader>		s_ShaderLib;
		static AssetLib<Texture2D>	s_Texture2DLib;
		static AssetLib<CubeMap>	s_CubeMapLib;
		static AssetLib<Mesh>		s_MeshLib;
		static AssetLib<Material>	s_MaterialLib;
		static AssetLib<Model>		s_ModelLib;

	};


}