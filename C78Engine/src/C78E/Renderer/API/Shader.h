#pragma once
#include <C78E/Renderer/Assets/Asset/Asset.h>

namespace C78E {

	class Shader : public Asset {
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void BindCompute(uint32_t x = 1, uint32_t y = 1, uint32_t z = 1) const = 0;
		virtual void unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> create(const FilePath filepath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> create(const std::string& name, const std::string& computeSrc);


		virtual Asset::AssetType getType() const override { return Asset::AssetType::Shader; }
		static AssetType getClassType() { return AssetType::Shader; };
	};
}
