#pragma once
#include <C78E/Utils/ParsingUtils.h>
#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Material/Material.h>

namespace C78E {

	namespace WavefrontParser {
		//std::regex{ "# ()" };
		bool parseComment(std::string_view& line, std::string_view& comment);
		//std::regex patternObject{ "o ([a-zA-Z_0-9]*)" };
		bool parseNewMesh(std::string_view line, std::string_view& objectName);
		//std::regex{ "newmtl ([a-zA-Z_0-9]*)" };
		bool parseNewMaterial(std::string_view line, std::string_view& materialName);
		//std::regex patternGroup{ "g ([a-zA-Z_0-9]*)" };
		bool parseGroup(std::string_view line, std::string_view& groupName);


		namespace WavefrontMaterialParser {
			//std::regex{ "usemtl ([a-zA-Z_0-9]*)" };
			bool parseUseMtl(std::string_view line, std::string_view& materialName);
			//std::regex{ "mtllib ([a-zA-Z_0-9]*)" };
			bool parseInclMtl(std::string_view line, std::string_view& fileName);

			//std::regex{ "illum ([0-9])" };
			bool parseMtlIllum(std::string_view line, Material::IlluminationModel& illuminationModel);

			namespace WavefrontBasicMaterialParser {
				bool parseMaterial(std::string_view section, Ref<BasicMaterial> basicMaterial);

				//std::regex{ "Ka ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlKambient(std::string_view line, glm::vec3& value);
				//std::regex{ "Kd ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlDiffuse(std::string_view line, glm::vec3& value);
				//std::regex{ "Ks ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlSpecular(std::string_view line, glm::vec3& value);
				//std::regex{ "Tf ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlTransmissionFilter(std::string_view line, glm::vec3& value);
				//std::regex{ "Ns ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlShininess(std::string_view line, glm::vec1& value);
				bool parseMtlShininess(std::string_view line, float& value);
				//std::regex{ "Ni ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlIndexOfRefraction(std::string_view line, glm::vec1& value);
				bool parseMtlIndexOfRefraction(std::string_view line, float& value);
				//std::regex{ "d ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				//std::regex{ "Tr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlDissolve(std::string_view line, glm::vec1& value);
				bool parseMtlDissolve(std::string_view line, float& value);

				//std::regex{ "map_Ka ([a-zA-Z_0-9]*)" };
				bool parseMtlMapAmbient(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Kd ([a-zA-Z_0-9]*)" };
				bool parseMtlMapDiffuse(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Ks ([a-zA-Z_0-9]*)" };
				bool parseMtlMapSpecular(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Ns ([a-zA-Z_0-9]*)" };
				bool parseMtlMapSpecularExponent(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_d ([a-zA-Z_0-9]*)" };
				bool parseMtlMapDissolve(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_bump ([a-zA-Z_0-9]*)" };
				bool parseMtlMapBump(std::string_view line, std::string_view& textureName);
				//std::regex{ "bump ([a-zA-Z_0-9]*)" };
				bool parseMtlBump(std::string_view line, std::string_view& textureName);
				//std::regex{ "disp ([a-zA-Z_0-9]*)" };
				bool parseMtlDisp(std::string_view line, std::string_view& textureName);
				//std::regex{ "decal ([a-zA-Z_0-9]*)" };
				bool parseMtlDecal(std::string_view line, std::string_view& textureName);

				//refl...
			};

			namespace WavefrontPBRMaterialParser {
				bool parseMaterial(std::string_view section, Ref<PBRMaterial> pBRMaterial);

				//std::regex{ "Pr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlRoughness(std::string_view line, glm::vec1& value);
				bool parseMtlRoughness(std::string_view line, float& value);
				//std::regex{ "Pm ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlMetallic(std::string_view line, glm::vec1& value);
				bool parseMtlMetallic(std::string_view line, float& value);
				//std::regex{ "Ps ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlSheen(std::string_view line, glm::vec1& value);
				bool parseMtlSheen(std::string_view line, float& value);
				//std::regex{ "Pc ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlClearcoatThickness(std::string_view line, glm::vec1& value);
				bool parseMtlClearcoatThickness(std::string_view line, float& value);
				//std::regex{ "Pcr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlClearcoatRoughness(std::string_view line, glm::vec1& value);
				bool parseMtlClearcoatRoughness(std::string_view line, float& value);
				//std::regex{ "Ke ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlEmissive(std::string_view line, glm::vec1& value);
				bool parseMtlEmissive(std::string_view line, float& value);
				//std::regex{ "aniso ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlAnisotropy(std::string_view line, glm::vec1& value);
				bool parseMtlAnisotropy(std::string_view line, float& value);
				//std::regex{ "anisor ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlAnisotropyRotation(std::string_view line, glm::vec1& value);
				bool parseMtlAnisotropyRotation(std::string_view line, float& value);

				//std::regex{ "map_Pr ([a-zA-Z_0-9]*)" };
				bool parseMtlMapRoughness(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Pm ([a-zA-Z_0-9]*)" };
				bool parseMtlMapMetallic(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Ps ([a-zA-Z_0-9]*)" };
				bool parseMtlMapSheen(std::string_view line, std::string_view& textureName);
				//std::regex{ "map_Ke ([a-zA-Z_0-9]*)" };
				bool parseMtlMapEmissive(std::string_view line, std::string_view& textureName);
				//std::regex{ "norm ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
				bool parseMtlMapNormal(std::string_view line, std::string_view& textureName);
			};



		};

		namespace WavefrontMeshParser {
			bool parseMesh(std::string_view section, Ref<Mesh> mesh, std::string_view& groupName);

			//std::regex{ "v ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))?" };
			bool parseVertex(std::string_view line, Mesh::Position& value);
			//std::regex{ "vn ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
			bool parseNormal(std::string_view line, glm::vec3& value);

			//std::regex{ "vp ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
			bool parseParam(std::string_view line, glm::vec2& value);
			//std::regex{ "vp ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
			bool parseParam(std::string_view line, glm::vec3& value);
			//std::regex{ "vt ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
			bool parseTextureCoordinate(std::string_view line, Mesh::TextureCoordinate& value);
			//std::regex{ "vt ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
			bool parseTextureCoordinate(std::string_view line, glm::vec3& value);


			using FaceData = std::array<std::pair<bool, std::vector<Mesh::Index>>, 3>;
			//std::regex{ "f( (([0-9]|[1-9][0-9]+)([]|\/)*)*)*" };
			bool parseFace(std::string_view line, FaceData& faceData);

			using LineData = std::vector<Mesh::Index>;
			//std::regex{ "l( ([0-9]|[1-9][0-9]+))( ([0-9]|[1-9][0-9]+))+" };
			bool parseLine(std::string_view line, LineData& lineData);
		};
	};

}