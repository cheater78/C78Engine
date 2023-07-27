/*
 * Renderer.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "DebugGL.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"


enum vSync { VSYNC = 1, GSYNC = -1, UNLIMITED = 0};
enum winMode { WINDOWED = 0, FULLSCREEN = 1, BORDERLESS = 2 };

class Renderer {
public:

	Renderer(std::string title, glm::vec2 dim, winMode fullscreen, vSync vsync);
	virtual ~Renderer();


	void newFrame(glm::vec4 clrRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) { rstDelta(); clear(clrRGBA); }

	float getDelta() { return mDelta; };

	void draw(const VertexArray& va, IndexBuffer& ib, Shader& shader, Camera& cam, glm::mat4 model);

	void endFrame();

	GLFWwindow* getWindow() { return m_Window; }
	glm::vec2 getWinDim() { return mDim; }

private:
	GLFWwindow* m_Window = nullptr;
	std::string mTitle;
	glm::vec2 mDim;
	winMode mFullscreen;
	vSync mVsync;
	double mLastFrameTime;
	float mDelta;

	void rstDelta();
	void clear(glm::vec4 clrRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
};

#endif /* RENDERER_H_ */
