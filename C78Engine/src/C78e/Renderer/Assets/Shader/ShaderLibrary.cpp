#include "C78EPCH.h"
#include "ShaderLibrary.h"

namespace C78E {
	Ref<ShaderLibrary> ShaderLibrary::s_ShaderLibrary;

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
	{
		C78_CORE_ASSERT(!exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	Ref<Shader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::create(filepath);
		add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
		C78_CORE_ASSERT(exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}