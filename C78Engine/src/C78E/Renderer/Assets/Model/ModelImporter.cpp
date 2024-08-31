#include "C78ePCH.h"
#include "ModelImporter.h"

#include <C78E/Project/Project.h>
#include <C78E/Renderer/Assets/AssetManager.h>
#include <C78E/Utils/Wavefront/WavefrontLoader.h>

namespace C78E {

	/*
	* ModelImporter::importModel
	* imports a Model, given its AssetMeta(filename, assetname gets written back)
	*/
	Ref<Model> ModelImporter::importModel(AssetHandle handle, const Asset::AssetMeta& meta) {
		FilePath ext = meta.fileSource.extension();
		if (ext == ".stl")
			return loadWavefrontModel(handle, meta);
		if (ext == ".gltf" || ext == ".glb")
			C78_CORE_ERROR("GLTF not implemented yet!");

		C78_CORE_ERROR("ModelImporter::importModel: File '{}' is not supported!", meta.fileSource);
		return nullptr;
	}




	/*
	* ModelImporter::loadWavefrontModel
	* uses WavefrontLoader to load a Wavefront model and registers all its Meshes and Materials in the current AssetManager
	* returns the resulting AssetHandle of the loaded Model
	*/
	Ref<Model> ModelImporter::loadWavefrontModel(AssetHandle handle, const Asset::AssetMeta& meta) {
		Ref<WavefrontLoader::WavefrontModel> wavefrontmodel = WavefrontLoader::loadModel(meta.fileSource);

		std::vector<Model::ModelPart> parts;

		for (auto& wfpart : wavefrontmodel->parts) {
			uint64_t meshID = wfpart.first;
			uint64_t materialID = wfpart.second;

			Model::ModelPart part;

			{
				Asset::AssetMeta meshMeta;
				Ref<Mesh> mesh = wavefrontmodel->meshes.at(meshID);

				meshMeta.type = Asset::AssetType::Mesh;
				meshMeta.fileSource = meta.fileSource;
				meshMeta.name = wavefrontmodel->meshNames.at(meshID);
				//part.m_Mesh = assetManager->addAsset(meshMeta, mesh);
			}

			{
				Asset::AssetMeta materialMeta;
				Ref<Material> material = wavefrontmodel->materials.at(materialID);

				materialMeta.type = Asset::AssetType::Material;
				materialMeta.fileSource = meta.fileSource;
				materialMeta.name = wavefrontmodel->materialNames.at(materialID);
				//part.m_Material = assetManager->addAsset(materialMeta, material);
			}

			parts.push_back(part);
		}

		return createRef<Model>(parts);
	}

}
