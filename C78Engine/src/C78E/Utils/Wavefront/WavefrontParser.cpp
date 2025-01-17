#include "C78EPCH.h"
#include "WavefrontParser.h"

namespace C78E {

	//std::regex{ "# ()" };
	bool WavefrontParser::parseComment(std::string_view& line, std::string_view& comment) {
		size_t it = line.find('#');
		if (it == std::string::npos)
			return false;
		comment = line.substr(it);
		line = line.substr(0, it);
		return true;
	}

	//std::regex{ "o ([a-zA-Z_0-9]*)" };
	bool WavefrontParser::parseNewMesh(std::string_view line, std::string_view& objectName) {
		static const std::string prefix = "o ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, objectName, prefix, suffix);
	}
	//std::regex{ "newmtl ([a-zA-Z_0-9]*)" };
	bool WavefrontParser::parseNewMaterial(std::string_view line, std::string_view& materialName) {
		static const std::string prefix = "newmtl ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, materialName, prefix, suffix);
	}

	//std::regex{ "g ([a-zA-Z_0-9]*)" };
	bool WavefrontParser::parseGroup(std::string_view line, std::string_view& groupName) {
		static const std::string prefix = "g ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, groupName, prefix, suffix);
	}

	/**
	* WavefrontMaterialParser
	*/
	bool WavefrontParser::WavefrontMaterialParser::parseUseMtl(std::string_view line, std::string_view& materialName) {
		static const std::string prefix = "usemtl ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, materialName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::parseInclMtl(std::string_view line, std::string_view& fileName) {
		static const std::string prefix = "mtllib ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, fileName, prefix, suffix);
	}

	bool WavefrontParser::WavefrontMaterialParser::parseMtlIllum(std::string_view line, Material::IlluminationModel& illuminationModel) {
		static const std::string prefix = "illum ";
		static const std::string suffix = "";
		glm::vec<1, size_t> value;
		const bool valid = TextParser::parseInfixVec<size_t, 1, prefix, suffix, ' ', TextParser::parseUnsignedInt>(line, value);
		if (valid)
			illuminationModel = static_cast<Material::IlluminationModel>(static_cast<uint8_t>(value.x));
		return valid;
	}

	/**
	* WavefrontBasicMaterialParser
	*/

	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMaterial(std::string_view section, Ref<BasicMaterial> basicMaterial) {
		std::vector<std::string_view> comments{ "" };
		for (std::string_view line : std::split(section, '\n')) {
			if (parseComment(line, comments.back())) { comments.emplace_back(""); }

			if (parseMtlKambient(line, basicMaterial->m_MaterialProperties.ambient)) continue;
			if (parseMtlDiffuse(line, basicMaterial->m_MaterialProperties.diffuse)) continue;
			if (parseMtlSpecular(line, basicMaterial->m_MaterialProperties.specular)) continue;
			if (parseMtlTransmissionFilter(line, basicMaterial->m_MaterialProperties.transmittance)) continue;
			if (parseMtlShininess(line, basicMaterial->m_MaterialProperties.shininess)) continue;
			if (parseMtlIndexOfRefraction(line, basicMaterial->m_MaterialProperties.ior)) continue;
			if (parseMtlDissolve(line, basicMaterial->m_MaterialProperties.dissolve)) continue;

			return false;
		}

		return true;
	}

	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlKambient(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "Ka ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlDiffuse(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "Kd ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlSpecular(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "Ks ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlTransmissionFilter(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "Tf ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlShininess(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Ns ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlShininess(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlShininess(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlIndexOfRefraction(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Ni ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlIndexOfRefraction(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlIndexOfRefraction(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlDissolve(std::string_view line, glm::vec1& value) {
		static const std::string prefixD = "d ";
		static const std::string prefixTr = "Tr ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefixD, suffix, ' ', TextParser::parseFloat>(line, value) ||
			TextParser::parseInfixVec<float, 1, prefixTr, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlDissolve(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlDissolve(line, buf);
		value = buf.x;
		return succ;
	}

	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapAmbient(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Ka ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapDiffuse(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Kd ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapSpecular(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Ks ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapSpecularExponent(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Ns ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapDissolve(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_d ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlMapBump(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_bump ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlBump(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "bump ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlDisp(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "disp ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontBasicMaterialParser::parseMtlDecal(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "decal ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}

	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMaterial(std::string_view section, Ref<PBRMaterial> pBRMaterial) {
		std::vector<std::string_view> comments{ "" };
		for (std::string_view line : std::split(section, '\n')) {
			if (parseComment(line, comments.back())) { comments.emplace_back(""); }

			if (parseMtlRoughness(line, pBRMaterial->m_MaterialProperties.roughness)) continue;
			if (parseMtlMetallic(line, pBRMaterial->m_MaterialProperties.metallic)) continue;
			if (parseMtlSheen(line, pBRMaterial->m_MaterialProperties.sheen)) continue;
			if (parseMtlClearcoatThickness(line, pBRMaterial->m_MaterialProperties.clearcoat_thickness)) continue;
			if (parseMtlClearcoatRoughness(line, pBRMaterial->m_MaterialProperties.clearcoat_roughness)) continue;
			if (parseMtlEmissive(line, pBRMaterial->m_MaterialProperties.emissive)) continue;
			if (parseMtlAnisotropy(line, pBRMaterial->m_MaterialProperties.anisotropy)) continue;
			if (parseMtlAnisotropyRotation(line, pBRMaterial->m_MaterialProperties.anisotropy_rotation)) continue;

			return false;
		}

		return true;
	}

	/**
	* WavefrontPBRMaterialParser
	*/
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlRoughness(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Pr ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlRoughness(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlRoughness(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMetallic(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Pm ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMetallic(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlMetallic(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlSheen(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Ps ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlSheen(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlSheen(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlClearcoatThickness(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Pc ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlClearcoatThickness(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlClearcoatThickness(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlClearcoatRoughness(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Pcr ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlClearcoatRoughness(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlClearcoatRoughness(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlEmissive(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "Ke ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlEmissive(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlEmissive(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlAnisotropy(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "aniso ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlAnisotropy(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlAnisotropy(line, buf);
		value = buf.x;
		return succ;
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlAnisotropyRotation(std::string_view line, glm::vec1& value) {
		static const std::string prefix = "anisor ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 1, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlAnisotropyRotation(std::string_view line, float& value) {
		glm::vec1 buf = glm::vec1{ value };
		const bool succ = parseMtlAnisotropyRotation(line, buf);
		value = buf.x;
		return succ;
	}

	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMapRoughness(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Pr ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMapMetallic(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Pm ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMapSheen(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Ps ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMapEmissive(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "map_Ke ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}
	bool WavefrontParser::WavefrontMaterialParser::WavefrontPBRMaterialParser::parseMtlMapNormal(std::string_view line, std::string_view& textureName) {
		static const std::string prefix = "norm ";
		static const std::string suffix = "";
		return TextParser::parseInfix(line, textureName, prefix, suffix);
	}



	/**
	* WavefrontMeshParser
	*/


	bool WavefrontParser::WavefrontMeshParser::parseMesh(std::string_view section, Ref<Mesh> mesh, std::string_view& groupName) {
		std::vector<std::string_view> comments{ "" };
		for (std::string_view line : std::split(section, '\n')) {
			if (parseComment(line, comments.back())) { comments.emplace_back(""); }
			if (parseGroup(line, groupName)) continue;


			Mesh::Position vertexPosition;
			if (parseVertex(line, vertexPosition)) {
				mesh->addPosition(vertexPosition);
				continue;
			}

			glm::vec3 normal;
			if (parseNormal(line, normal)) {
				mesh->addNormal(Mesh::Normal(normal, 0.f));
				continue;
			}

			glm::vec3 textureCoordinate;
			if (parseTextureCoordinate(line, textureCoordinate)) {
				mesh->addTextureCoordinate(Mesh::TextureCoordinate(textureCoordinate)); //TODO: 3rd component ignored
				continue;
			}

			//TODO: Param vertecies

			FaceData faceData;
			if (parseFace(line, faceData)) {
				const size_t vertexCount = faceData[0].second.size();
				std::vector<Mesh::HalfedgeIndex> faceHalfedges = std::vector<Mesh::HalfedgeIndex>(vertexCount);
				for (size_t i = 0; i < vertexCount; i++)
					faceHalfedges[i] = mesh->addHalfedge(faceData[0].second[i], faceData[0].second[(i + 1) % vertexCount]);
				mesh->addFace(faceHalfedges);
				continue;
			}

			LineData lineData;
			if (parseLine(line, lineData)) {
				const size_t vertexCount = lineData.size();
				std::vector<Mesh::HalfedgeIndex> lineHalfedges = std::vector<Mesh::HalfedgeIndex>(vertexCount);
				for (size_t i = 0; i < vertexCount; i++)
					lineHalfedges[i] = mesh->addHalfedge(lineData[i], lineData[(i + 1) % vertexCount]);
				continue;
			}

		}
		return true;
	}

	//std::regex{ "v ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))?" };
	bool WavefrontParser::WavefrontMeshParser::parseVertex(std::string_view line, Mesh::Position& value) {
		static const std::string prefix = "v ";
		static const std::string suffix = "";
		Mesh::Position position = { 0.f, 0.f, 0.f, 1.f };
		const bool succ = TextParser::parseInfixVec<float, 4, prefix, suffix, ' ', TextParser::parseFloat>(line, position) ||
			TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, *(glm::vec3*)&position);
		if (position.w != 0.f)
			position /= position.w;
		return succ;
	}

	bool WavefrontParser::WavefrontMeshParser::parseNormal(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "vn ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	//std::regex{ "vp ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
	bool WavefrontParser::WavefrontMeshParser::parseParam(std::string_view line, glm::vec2& value) {
		static const std::string prefix = "vp ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 2, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	//std::regex{ "vp ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
	bool WavefrontParser::WavefrontMeshParser::parseParam(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "vp ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	//std::regex{ "vt ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
	bool WavefrontParser::WavefrontMeshParser::parseTextureCoordinate(std::string_view line, Mesh::TextureCoordinate& value) {
		static const std::string prefix = "vt ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 2, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	//std::regex{ "vt ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
	bool WavefrontParser::WavefrontMeshParser::parseTextureCoordinate(std::string_view line, glm::vec3& value) {
		static const std::string prefix = "vt ";
		static const std::string suffix = "";
		return TextParser::parseInfixVec<float, 3, prefix, suffix, ' ', TextParser::parseFloat>(line, value);
	}

	//std::regex{ "f( (([0-9]|[1-9][0-9]+)([]|\/)*)*)*" };
	bool WavefrontParser::WavefrontMeshParser::parseFace(std::string_view line, FaceData& faceData) {
		static const std::string prefix = "f ";
		static const std::string suffix = "";
		std::string_view infix;
		if (!TextParser::parseInfix(line, infix, prefix, suffix)) return false;

		const auto outerStrings = std::split(infix, ' ');
		const size_t vertexCount = outerStrings.size();
		if (vertexCount < 3) return false;
		for (auto& prop : faceData) {
			prop.first = true;
			prop.second.resize(vertexCount);
		}

		for (size_t outerElement = 0; outerElement < vertexCount; outerElement++) {
			const auto innerStrings = std::split(outerStrings[outerElement], '/');
			if (innerStrings.size() > 3) return false;
			for (size_t innerElement = 0; innerElement < 3; innerElement++) {
				Mesh::Index propElemIndex = -1;
				if (faceData[innerElement].first &= TextParser::parseUnsignedInt(innerStrings[innerElement], propElemIndex))
					faceData[innerElement].second[outerElement] = propElemIndex;
				else if (innerElement == 0) return false;
			}
		}
		return true;
	}

	//std::regex{ "l( ([0-9]|[1-9][0-9]+))( ([0-9]|[1-9][0-9]+))+" };
	bool WavefrontParser::WavefrontMeshParser::parseLine(std::string_view line, LineData& lineData) {
		static const std::string prefix = "l ";
		static const std::string suffix = "";
		std::string_view infix;
		if (!TextParser::parseInfix(line, infix, prefix, suffix)) return false;

		const auto indexStrings = std::split(infix, ' ');
		const size_t vertexCount = indexStrings.size();
		if (vertexCount < 3) return false;
		lineData.resize(vertexCount);

		for (size_t element = 0; element < vertexCount; element++) {
			Mesh::Index index = -1;
			if (!TextParser::parseUnsignedInt(indexStrings[element], index)) return false;
			lineData[element] = index;
		}
		return true;
	}

}