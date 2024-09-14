#include "C78ePCH.h"
#include "ModelImporter.h"

#include <C78E/Utils/Wavefront/WavefrontLoader.h>

namespace C78E {

	/*
	* ModelImporter::importModel
	* imports a Model, given its AssetMeta(filename, assetname gets written back)
	*/
	Ref<Model> ModelImporter::importModel(AssetHandle handle, const Asset::AssetMeta& meta) {
		FilePath ext = meta.fileSource.extension();
		if (ext == ".obj")
			return loadWavefrontModel(handle, meta, 1)->models().front();
		if (ext == ".gltf" || ext == ".glb")
			C78_CORE_ERROR("GLTF not implemented yet!");

		C78_CORE_ERROR("ModelImporter::importModel: File '{}' is not supported!", meta.fileSource);
		return nullptr;
	}

	Ref<ModelComposite> ModelImporter::importModelComposite(AssetHandle handle, const Asset::AssetMeta& meta) {
		FilePath ext = meta.fileSource.extension();
		if (ext == ".obj")
			return loadWavefrontModel(handle, meta);
		if (ext == ".gltf" || ext == ".glb")
			C78_CORE_ERROR("GLTF not implemented yet!");

		C78_CORE_ERROR("ModelImporter::importModelComposite: File '{}' is not supported!", meta.fileSource);
		return nullptr;
	}




	/*
	* ModelImporter::loadWavefrontModel
	* uses WavefrontLoader to load a Wavefront model and loads all its Meshes and Materials
	* returns the resulting ModelComposite of the loaded Model
	*/
	Ref<ModelComposite> ModelImporter::loadWavefrontModel(AssetHandle handle, const Asset::AssetMeta& meta, const uint32_t maxParts) {
		Ref<WavefrontLoader::WavefrontModel> wavefrontmodel = WavefrontLoader::loadModel(meta.fileSource);
		
		std::vector<Ref<Model>> models;
		for (auto& wfpart : wavefrontmodel->parts) {
			uint64_t meshID = wfpart.first;
			uint64_t materialID = wfpart.second;

			Ref<Mesh> mesh = wavefrontmodel->meshes.at(meshID);
			Ref<Material> material = wavefrontmodel->materials.at(materialID);
			models.push_back(createRef<Model>(mesh, material));

			if (maxParts && models.size() >= maxParts) break;
		}

		return createRef<ModelComposite>(models);
	}

}
