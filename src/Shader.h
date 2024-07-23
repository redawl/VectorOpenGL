#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>

// Shader blobs
extern uint8_t shader_vs[];
extern int shader_vs_size;
extern uint8_t shader_gs[];
extern int shader_gs_size;
extern uint8_t shader_fs[];
extern int shader_fs_size;

class Shader {
public:
	unsigned int ID;

	Shader(std::string xEquation, std::string yEquation);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
    std::string vShaderCode = std::string((char *) shader_vs, shader_vs_size);
    std::string fShaderCode = std::string((char *) shader_fs, shader_fs_size);
    std::string gShaderCode = std::string((char *) shader_gs, shader_gs_size);
	// Helper methods
	std::string readFile(const char * filePath) const {
		std::ifstream fileStream;
		std::stringstream fileStringStream;
		std::string fileContents;
		fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			fileStream.open(filePath);
			fileStringStream << fileStream.rdbuf();
			fileStream.close();
			fileContents = fileStringStream.str();
			
			return fileContents;
		} catch (std::ifstream::failure e){
			throw new std::runtime_error("ERROR: File not successfully read");
		}

		throw new std::runtime_error("ERROR: This should never happen");
	}

	unsigned int compileShader(GLuint SHADER_TYPE, const char * shaderCode) {
		unsigned int shader;
		int success;
		char infoLog[512];

		// Create shader object
		shader = glCreateShader(SHADER_TYPE);
		// Replace source code in shader object
		// 1 = number of elements in string array, NULL for no length of string (Assumed to be null terminated)
		glShaderSource(shader, 1, &shaderCode, NULL);

		// Actually compile the shader
		glCompileShader(shader);

		// Retrieve status of compilation
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			throw new std::runtime_error(infoLog);
		}

		return shader;
	}
};
#endif
