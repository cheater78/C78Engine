#pragma once

#include "C78e/Renderer/Assets/Shader/Shader.h"

namespace C78E {

	class ShaderLibrary {
	public:
		static Ref<ShaderLibrary> create() { s_ShaderLibrary = createRef<ShaderLibrary>(); return s_ShaderLibrary; }
		static Ref<ShaderLibrary> get() { if (!s_ShaderLibrary) return create(); else return s_ShaderLibrary; }

		ShaderLibrary() = default;
		ShaderLibrary(const ShaderLibrary&) = delete;
		~ShaderLibrary() = default;

		void add(const std::string& name, const Ref<Shader>& shader);
		void add(const Ref<Shader>& shader);
		Ref<Shader> load(const std::string& filepath);
		Ref<Shader> load(const std::string& name, const std::string& filepath);

		Ref<Shader> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;

		static Ref<ShaderLibrary> s_ShaderLibrary;
	};

}