/*
 * Shader.cpp
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#include "Shader.h"

#include <fstream>
#include <string>
#include <sstream>
#include "DebugGL.h"


Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {
	ShaderProgramSource source = parseShader(m_FilePath);
	m_RendererID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const {
	GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3){
	GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}


void Shader::setUniform1i(const std::string& name, int i0){
	GLCall(glUniform1i(getUniformLocation(name), i0));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix){
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}













/// PRIVATE---------------------------------------------------------------------------------------------------------

ShaderProgramSource Shader::parseShader(std::string filepath){
	enum class ShaderType{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};


	std::fstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;


	while(getline(stream, line)){
		if(line.find("#shader") != std::string::npos){
			if(line.find("vertex") != std::string::npos){
				type = ShaderType::VERTEX;
			} else
			if(line.find("fragment") != std::string::npos){
				type = ShaderType::FRAGMENT;
			}

		}
		else {
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str() , ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source){
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int succ;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &succ));
	if(!succ){
		int len;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char* msg = (char*)alloca(len*sizeof(char));
		GLCall(glGetShaderInfoLog(id, len, &len, msg));
		std::cout << "SHADER FAILED TO COMPILE! (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << ")" << std::endl;
		std::cout << msg << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::createShader(std::string& vertexShader, std::string& fragmentShader){
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

int Shader::getUniformLocation(const std::string& name){
	if(m_UniformLocCache.find(name) != m_UniformLocCache.end()){
		return m_UniformLocCache[name];
	}

	GLCall(int loc = glGetUniformLocation(m_RendererID, name.c_str()));
	if(loc == -1) std::cout << "[OpenGL] Uniform WARN: Uniform " << name << " not found!" << std::endl;
	m_UniformLocCache[name] = loc;
	return loc;
}
