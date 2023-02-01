#pragma once

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>

#include <string>
#include <vector>

// Wrapper for graphics api shaders
class Shader
{
public:
	// Read and build the shader
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath = nullptr);

	// Activate the shader
	void Use() const;

	// Uniform functions
	void SetVar(const char *name, bool value) const;
	void SetVar(const char *name, int value) const;
	void SetVar(const char *name, float value) const;
	void SetVar(const char *name, const glm::vec2 &value) const;
	void SetVar(const char *name, const glm::vec3 &value) const;
	void SetVar(const char *name, const glm::mat3 &value) const;
	void SetVar(const char *name, const glm::mat4 &value) const;

private:
	// Read text from file
	std::string GetShaderCode(const char *path) const;

	// Compile shader from text into id 
	void CompileShader(GLuint shaderID, const char *content) const;

	// Link multiple compiled shaders
	static GLuint LinkShaders(const std::vector<GLuint> &shaders);

	unsigned int id_;
};