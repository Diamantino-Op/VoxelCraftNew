#pragma once

#include "Texture.h"

// General purpose framebuffer wrapper class
class Framebuffer
{
public:
	Framebuffer(int width, int height);	// Create framebuffer with width and height
	Framebuffer(Framebuffer &&other);	// Move
	void BindFramebuffer() const;				// Bind the framebuffer for drawing
	Texture &GetTexture();				// Get the texture that was drawn to (depth)
	~Framebuffer();

private:
	GLuint fbo_{};
	Texture depthBuffer_;
	int width_, height_;
};

