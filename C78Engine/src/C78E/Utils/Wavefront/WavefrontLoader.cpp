#include <C78EPCH.h>
#include "WavefrontLoader.h"

namespace C78E {

   

	Ref<Asset::Group> WavefrontLoader::importWavefront(const FilePath& source) {
		return importWavefront(FileSystem::readFileText(source));
	}

	Ref<Asset::Group> WavefrontLoader::importWavefront(const std::string& source) {
		Ref<Asset::Group> assets = createRef<Asset::Group>();

		std::vector<std::string_view> containedAssetSources;

		size_t objBegin = -1;
		size_t lineChrNum = 0;
		for (std::string line; std::getline(std::istringstream(source), line); lineChrNum += line.size()) {
			std::smatch match;
			if (std::regex_match(line, match, patternObject) && match.size() == 1 || lineChrNum + line.size() >= source.size() - 1) {
				const size_t objStart = objBegin;
				const size_t objEnd = lineChrNum - 1;
				objBegin = lineChrNum;
				if (objBegin == -1) continue;

				Ref<Mesh> mesh = createRef<Mesh>();
				Ref<Mesh::Meta> meta = createRef<Mesh::Meta>();

				meta->name = match[0].str();

				importMesh(mesh, meta, std::string_view(source.begin() + objStart, source.begin() + objEnd));

				assets->at(std::static_pointer_cast<Asset>(mesh)) = meta;
			}
		}

		return assets;
	}

	bool WavefrontLoader::importMesh(Ref<Mesh> mesh, Ref<Asset::Meta> meta, std::string_view source) {
		mesh->clear();
		for (std::string line; std::getline(std::istringstream(source.data()), line);) {
			std::smatch match;
			if (std::regex_match(line, match, patternVertex) && match.size() == 4) {
				mesh->addPosition(Mesh::Position{ std::stof(match[0].str()), std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[4].str()) });
			} else if (std::regex_match(line, match, patternNormal) && match.size() == 4) {
				mesh->addNormal(Mesh::Normal{ std::stof(match[0].str()), std::stof(match[1].str()), std::stof(match[2].str()), std::stof(match[4].str()) });
			} else if (std::regex_match(line, match, patternTextureCoordinate) && match.size() == 2) {
				mesh->addTextureCoordinate(Mesh::TextureCoordinate{ std::stof(match[0].str()), std::stof(match[1].str()) });
			} else if (std::regex_match(line, match, patternParam) && match.size() == 3) {
				mesh->addColor(Mesh::Color{ std::stof(match[0].str()), std::stof(match[1].str()), std::stof(match[2].str()) , 1.f });
			} else if (std::regex_match(line, match, patternFace)) {
				const std::vector<std::string> vertecies = std::split(match.str().substr(3), ' ');

				std::vector<Mesh::HalfedgeIndex> halfedgeIndecies;
				Mesh::VertexIndex from = -1;
				for (auto it = vertecies.begin(); it != vertecies.end(); it++) {
					const auto components = std::split(*it, "/");
					if (!components.size()) break;
					const bool hasTextureCoords = components.size() != 1 && it->find("//") == std::string::npos;
					const bool hasNormals = it->find("//") != std::string::npos || components.size() == 3;
					const Mesh::VertexIndex to = std::stoul(components[0]);
					halfedgeIndecies.emplace_back(mesh->addHalfedge(from, to));
					from = to;
					if (hasTextureCoords)
						mesh->textureCoordinateIndex(halfedgeIndecies.back()) = std::stoul(components[1]);
					if (hasNormals)
						mesh->normalIndex(halfedgeIndecies.back()) = std::stoul(components[2]);
					
				}
				if (halfedgeIndecies.size() < 3) continue;
				mesh->addFace(halfedgeIndecies);

			}
				
		}
		return false;
	}

	bool WavefrontLoader::importMaterial(Ref<Material> mesh, Ref<Asset::Meta> meta, std::string_view source) {
		return false;
	}

}