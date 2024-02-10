#pragma once

#include "C78e/Core/Types.h"
#include <C78e/Renderer/Assets/Shader/Shader.h>

namespace C78E {

	class Material {
	public:
		Material() = default;
		Material(Ref<Shader> shader, MaterialProperties props);
		Material(std::string filename, MaterialProperties props);
		Material(const Material&) = default;

		Material(std::string filename) { /* dummy for AssetLib, add later */ }

		void setShader(Ref<Shader> shader) { m_Shader = shader; }
		Ref<Shader>& getShader() { return m_Shader; }

		MaterialProperties& getProperties() { return m_Material; }
		EntityMaterialUniform& getUniform() { return m_Material; }

	private:
		Ref<Shader> m_Shader;
		MaterialProperties m_Material;
	};
}