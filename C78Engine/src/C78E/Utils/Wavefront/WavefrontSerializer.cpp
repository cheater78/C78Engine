#include <C78EPCH.h>
#include "WavefrontSerializer.h"
#include "WavefrontParser.h"

namespace C78E {

	Ref<Asset::Group> WavefrontSerializer::importWavefront(const FilePath& source) {
		C78E_CORE_INFO("Importing Wavefront from File: {}", source.string());
		return importWavefront(FileSystem::readFileText(source));
	}

	Ref<Asset::Group> WavefrontSerializer::importWavefront(const std::string& source) {
		Ref<Asset::Group> assets = createRef<Asset::Group>();

		std::vector<std::string_view> containedAssetSources;

		size_t objBegin = -1;
		size_t lineChrNum = 0;
		for (std::string line; std::getline(std::istringstream(source), line); lineChrNum += line.size()) {
			bool lastLine = lineChrNum + line.size() >= source.size() - 1;
			
			std::string_view meshName;
			if (WavefrontParser::parseNewMesh(line, meshName) || lastLine) {
				const size_t objStart = objBegin;
				const size_t objEnd = lineChrNum - 1;
				objBegin = lineChrNum;
				if (objBegin == -1) continue;

				Ref<Mesh> mesh = createRef<Mesh>();
				Ref<Mesh::Meta> meta = createRef<Mesh::Meta>();
				meta->name = meshName;

				if(importMesh(mesh, meta, std::string_view(source.begin() + objStart, source.begin() + objEnd)));
					assets->at(std::static_pointer_cast<Asset>(mesh)) = meta;
					continue;
			}

			std::string_view materialName;
			if (WavefrontParser::parseNewMaterial(line, materialName) || lastLine) {
				const size_t objStart = objBegin;
				const size_t objEnd = lineChrNum - 1;
				objBegin = lineChrNum;
				if (objBegin == -1) continue;

				C78E_CORE_WARN("WavefrontSerializer::importWavefront: no material importer yet!");
			}
		}

		return assets;
	}

	bool WavefrontSerializer::importMesh(Ref<Mesh> mesh, Ref<Asset::Meta> meta, std::string_view source) {
		std::string_view groupName;
		return WavefrontParser::WavefrontMeshParser::parseMesh(source, mesh, groupName);
	}

	bool WavefrontSerializer::importMaterial(Ref<Material> mesh, Ref<Asset::Meta> meta, std::string_view source) {
		return false;
	}


}