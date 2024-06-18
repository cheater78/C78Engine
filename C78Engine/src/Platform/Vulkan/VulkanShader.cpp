#include "C78EPCH.h"
#include "VulkanShader.h"

namespace C78E {

	VulkanShader::VulkanShader(const std::string& filepath) {
	}

	VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc){
	}

	VulkanShader::VulkanShader(const std::string& name, const std::string& computeSrc){
	}

	VulkanShader::~VulkanShader() {
	}

	void VulkanShader::bind() const {
	}

	void VulkanShader::BindCompute(uint32_t x, uint32_t y, uint32_t z) const {
	}

	void VulkanShader::unbind() const {
	}

	void VulkanShader::SetInt(const std::string& name, int value) {
		UploadUniformInt(name, value);
	}

	void VulkanShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
		UploadUniformIntArray(name, values, count);
	}

	void VulkanShader::SetFloat(const std::string& name, float value) {
		UploadUniformFloat(name, value);
	}

	void VulkanShader::SetFloat2(const std::string& name, const glm::vec2& value) {
		UploadUniformFloat2(name, value);
	}

	void VulkanShader::SetFloat3(const std::string& name, const glm::vec3& value) {
		UploadUniformFloat3(name, value);
	}

	void VulkanShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		UploadUniformFloat4(name, value);
	}

	void VulkanShader::SetMat4(const std::string& name, const glm::mat4& value) {
		UploadUniformMat4(name, value);
	}

	void VulkanShader::UploadUniformInt(const std::string& name, int value) {
	}

	void VulkanShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
	}

	void VulkanShader::UploadUniformFloat(const std::string& name, float value) {
	}

	void VulkanShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
	}

	void VulkanShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
	}

	void VulkanShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
	}

	void VulkanShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
	}

	void VulkanShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
	}

}
