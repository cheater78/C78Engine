#include "C78EPCH.h"
#include "Material.h"


namespace C78E {

	Material::Material(Ref<Shader> shader, MaterialProperties props)
		: m_Shader(shader), m_Material(props)
	{ }

	Material::Material(std::string filename, MaterialProperties props)
		: m_Material(props)
	{ }

}