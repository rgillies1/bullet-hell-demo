#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	int programID;
	bool compiled;
	std::string compilerOutput;
	void compileShader(int shader, const char* source);
public:
	Shader(const char* vertex, const char* fragment, const char* geometry = nullptr);
	int getID();
	// Compiler checks
	bool isCompiled();
	std::string getCompilerOutput();
	// Use shader
	void use();
	// Uniform setters
	void setFloat(const char* uniformName, float valueToSet);
	void setInt(const char* uniformName, int valueToSet);
	void setVector2f(const char* uniformName, float xVal, float yVal);
	void setVector3f(const char* uniformName, float xVal, float yVal, float zVal);
	void setVector4f(const char* uniformName, float xVal, float yVal, float zVal, float wVal);
	void setMatrix2(const char* uniformName, glm::mat2 newMatrix);
	void setMatrix3(const char* uniformName, glm::mat3 newMatrix);
	void setMatrix4(const char* uniformName, glm::mat4 newMatrix);
};

#endif