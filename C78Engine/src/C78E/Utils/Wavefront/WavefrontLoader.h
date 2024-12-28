#pragma once

#include <C78E/Renderer/Assets/Mesh/Mesh.h>
#include <C78E/Renderer/Assets/Material/Material.h>
#include <C78E/Renderer/API/Texture.h>

namespace C78E {

	class WavefrontLoader {
	public:
		Ref<Asset::Group> importWavefront(const FilePath& source);
		Ref<Asset::Group> importWavefront(const std::string& source);

		bool importMesh(Ref<Mesh> mesh, Ref<Asset::Meta> meta, std::string_view source);
		bool importMaterial(Ref<Material> mesh, Ref<Asset::Meta> meta, std::string_view source);
	private:

		std::regex patternComment{"# ()"};
		
		std::regex patternNewMtl{"newmtl ([a-zA-Z_0-9]*)"};
		std::regex patternUseMtl{"usemtl ([a-zA-Z_0-9]*)"};
		std::regex patternincludeMtl{"mtllib ([a-zA-Z_0-9]*)"};

		std::regex patternMtlKambient{ "Ka ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlKdiffuse{ "Kd ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlKspecular{ "Ks ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlSpecComp{ "Ns ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtld{ "d ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlTr{ "Tr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlTf{ "Tf ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]? ([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0- 9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlNi{ "Ni ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlIllum{ "illum ([0-9])" };

		std::regex patternMtlMapKa{ "map_Ka ([a-zA-Z_0-9]*)" };
		std::regex patternMtlMapKd{ "map_Kd ([a-zA-Z_0-9]*)" };
		std::regex patternMtlMapKs{ "map_Ks ([a-zA-Z_0-9]*)" };
		std::regex patternMtlMapNs{ "map_Ns ([a-zA-Z_0-9]*)" };
		std::regex patternMtlMapd{ "map_d ([a-zA-Z_0-9]*)" };
		std::regex patternMtlMapBump{ "map_bump ([a-zA-Z_0-9]*)" };
		std::regex patternMtlBump{ "bump ([a-zA-Z_0-9]*)" };
		std::regex patternMtlDisp{ "disp ([a-zA-Z_0-9]*)" };
		std::regex patternMtlDecal{ "decal ([a-zA-Z_0-9]*)" };

		std::regex patternMtlPBRPr{ "Pr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRPm{ "Pm ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRPs{ "Ps ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRPc{ "Pc ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRPcr{ "Pcr ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRKe{ "Ke ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRaniso{ "aniso ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRanisor{ "anisor ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };
		std::regex patternMtlPBRnorm{ "norm ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))" };

		std::regex patternMtlPBRMapPr{ "map_Pr ([a-zA-Z_0-9]*)" };
		std::regex patternMtlPBRMapPm{ "map_Pm ([a-zA-Z_0-9]*)" };
		std::regex patternMtlPBRMapPs{ "map_Ps ([a-zA-Z_0-9]*)" };
		std::regex patternMtlPBRMapKe{ "map_Ke ([a-zA-Z_0-9]*)" };


		std::regex patternObject{"o ([a-zA-Z_0-9]*)"};
		std::regex patternGroup{"g ([a-zA-Z_0-9]*)"};

		std::regex patternVertex{"v ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))?"};
		std::regex patternNormal{"vn ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))"};
		std::regex patternParam{"vp ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))?"};
		std::regex patternTextureCoordinate{"vt ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) ([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*)) *([-+]?([0]\.[0-9]*|[1-9]*[0-9]*\.[0-9]*))?"};

		std::regex patternFace{"f( (([0-9]|[1-9][0-9]+)([]|\/)*)*)*"};
		std::regex patternLine{"l( ([0-9]|[1-9][0-9]+))( ([0-9]|[1-9][0-9]+))+" };
	};

}