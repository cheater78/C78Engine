/*
 * DebugGL.cpp
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#include "DebugGL.h"

void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* file, int line){
	bool ret = true;
	while(GLenum error = glGetError()){
		std::cout << "[OpenGL] (" << error << ") > in " << file << " : " << func << " : " << line << std::endl;
		ret = false;
	}
	return ret;
}




