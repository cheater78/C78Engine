/*
 * Texture.h
 *
 *  Created on: Sep 19, 2022
 *      Author: j
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

class Texture {
public:
	Texture(const std::string& filepath);
	virtual ~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() const { return m_Width; }
	inline int getHeight() const { return m_Height; }

private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};

#endif /* TEXTURE_H_ */
