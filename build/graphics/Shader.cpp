#include <exception>

#include "Shader.h"
#include <iostream>

const int COMPILER_MSG_SIZE = 1024;

Shader::Shader(const char* vertex, const char* fragment, const char* geometry)
{
	if (vertex == nullptr || fragment == nullptr) 
		throw std::exception("Shader source cannot be nullptr");
	if (strlen(geometry) == 0) geometry = nullptr;
	compiled = false;

	int vertexID, fragmentID, geometryID;
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexID, vertex);
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentID, fragment);
	if (geometry != nullptr)
	{
		geometryID = glCreateShader(GL_GEOMETRY_SHADER);
		compileShader(geometryID, geometry);
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	if (geometry != nullptr) glAttachShader(programID, geometryID);
	glLinkProgram(programID);
	int success;
	char infoLog[COMPILER_MSG_SIZE];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, COMPILER_MSG_SIZE, NULL, infoLog);
		compilerOutput.append(infoLog + '\n');
		throw std::exception(compilerOutput.c_str());
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if (geometry != nullptr)
		glDeleteShader(geometryID);
	if (compilerOutput.size() > 0) compiled = false;
	else compiled = true;
}

int Shader::getID()
{
	return programID;
}

bool Shader::isCompiled()
{
	return compiled;
}

std::string Shader::getCompilerOutput()
{
	return compilerOutput;
}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::setFloat(const char* uniformName, float valueToSet)
{
	glUniform1f(glGetUniformLocation(this->programID, uniformName), valueToSet);
}

void Shader::setInt(const char* uniformName, int valueToSet)
{
	glUniform1i(glGetUniformLocation(this->programID, uniformName), valueToSet);
}

void Shader::setVector2f(const char* uniformName, float xVal, float yVal)
{
	glUniform2f(glGetUniformLocation(this->programID, uniformName), xVal, yVal);
}

void Shader::setVector3f(const char* uniformName, float xVal, float yVal, float zVal)
{
	glUniform3f(glGetUniformLocation(this->programID, uniformName), xVal, yVal, zVal);
}

void Shader::setVector4f(const char* uniformName, float xVal, float yVal, float zVal, float wVal)
{
	glUniform4f(glGetUniformLocation(this->programID, uniformName), xVal, yVal, zVal, wVal);
}

void Shader::setMatrix2(const char* uniformName, glm::mat2 newMatrix)
{
	glUniformMatrix2fv(glGetUniformLocation(this->programID, uniformName), 1, false, glm::value_ptr(newMatrix));
}

void Shader::setMatrix3(const char* uniformName, glm::mat3 newMatrix)
{
	glUniformMatrix3fv(glGetUniformLocation(this->programID, uniformName), 1, false, glm::value_ptr(newMatrix));
}

void Shader::setMatrix4(const char* uniformName, glm::mat4 newMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(this->programID, uniformName), 1, false, glm::value_ptr(newMatrix));
}

void Shader::compileShader(int shader, const char* source)
{
	int success = 0;
	char infoLog[COMPILER_MSG_SIZE];
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, COMPILER_MSG_SIZE, NULL, infoLog);
		compilerOutput.append(infoLog + '\n');
		throw std::exception(infoLog);
	}
}