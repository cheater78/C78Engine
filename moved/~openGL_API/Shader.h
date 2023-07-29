/*
 * Shader.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


struct ShaderProgramSource{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader {
public:
	Shader(const std::string& filepath);
	virtual ~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int i0);
	void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	std::string m_FilePath;
	unsigned int m_RendererID;


	std::unordered_map<std::string, int> m_UniformLocCache;
	int getUniformLocation(const std::string& name);

	ShaderProgramSource parseShader(std::string filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(std::string& vertexShader, std::string& fragmentShader);

};

#endif /* SHADER_H_ */
