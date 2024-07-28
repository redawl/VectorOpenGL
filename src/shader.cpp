#include "shader.h"

Shader::Shader(std::string xEquation, std::string yEquation) {
	// compile shaders
	unsigned int vertex, fragment, geometry;
	
	vertex   = compileShader(GL_VERTEX_SHADER, vShaderCode.c_str());
	fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode.c_str());

	gShaderCode.replace(gShaderCode.find("X_EQUATION"), sizeof("X_EQUATION") - 1, xEquation);
	gShaderCode.replace(gShaderCode.find("Y_EQUATION"), sizeof("Y_EQUATION") - 1, yEquation);

	geometry = compileShader(GL_GEOMETRY_SHADER, gShaderCode.c_str());

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glAttachShader(ID, geometry);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		throw new std::runtime_error(infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
