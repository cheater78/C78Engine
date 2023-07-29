/*
 * Renderer.cpp
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#include "Renderer.h"
#include <iostream>

Renderer::Renderer(std::string title, glm::vec2 dim, winMode fullscreen, vSync vsync) : mTitle(title), mDim(dim), mFullscreen(fullscreen), mVsync(vsync)
{
	glfwInit();
	if (mFullscreen != WINDOWED) {
		glfwWindowHint(GLFW_DECORATED, GLFW_NO_API); // Remove the border and titlebar..   
		mDim.x = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
		mDim.y = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
		m_Window = glfwCreateWindow(mDim.x, mDim.y, mTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
		
	}
	else
	{
		m_Window = glfwCreateWindow(mDim.x, mDim.y, mTitle.c_str(), NULL, NULL);
	}
	glfwMakeContextCurrent(m_Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	GLCall(glfwSwapInterval(mVsync));
	//Blending
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//3D depth
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	mLastFrameTime = glfwGetTime();
}

Renderer::~Renderer(){
	glfwTerminate();
}

void Renderer::rstDelta() {
	mDelta = glfwGetTime() - mLastFrameTime;
	mLastFrameTime += mDelta;
}

void Renderer::clear(glm::vec4 clrRGBA){
	GLCall(glClearColor(clrRGBA.r, clrRGBA.g, clrRGBA.b, clrRGBA.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, IndexBuffer& ib, Shader& shader, Camera& cam, glm::mat4 model){
	shader.bind();
	va.bind();
	ib.bind();
	shader.setUniformMat4f("MVP", cam.getProjView(mDim) * model);
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::endFrame(){
	int display_w, display_h;
	GLCall(glfwGetFramebufferSize(m_Window, &display_w, &display_h));
	GLCall(glViewport(0, 0, display_w, display_h));
	mDim = glm::vec2(display_w, display_h);
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}
