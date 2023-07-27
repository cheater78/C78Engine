/*
 * DebugGL.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef DEBUGGL_H_
#define DEBUGGL_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
//#include <signal.h>

#define ASSERT(x) if(!(x)){ std::cout << "[ASSERT triggered!] program exit!" << std::endl; /*raise(SIGTRAP);*/ }
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* func, const char* file, int line);



#endif /* DEBUGGL_H_ */
