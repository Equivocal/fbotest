#include "shader.h"

namespace nex {
	void Shader::create(std::string file, GLuint shaderType) {
		std::ifstream infile;
		infile.open(file);

		std::string src;
		src.assign(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

		const char* c_str = src.c_str();

		this->m_shader = glCreateShader(shaderType);
		if (m_shader == 0) {
			printf("Error creating shader: %s\n", glewGetErrorString(glGetError()));
		}
		assert(glGetError() == GL_NO_ERROR);
#ifdef _WIN32
		glShaderSource(this->m_shader, 1, (const GLchar*const*)&c_str, NULL);
#else
		glShaderSource(this->m_shader, 1, &c_str, NULL);
		assert(glGetError() == GL_NO_ERROR);
#endif
		glCompileShader(this->m_shader);
		assert(glGetError() == GL_NO_ERROR);

		GLint result;
		glGetShaderiv(this->m_shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			printf("Vertex shader compilation failed");
			GLint logLen;
			glGetShaderiv(this->m_shader, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0) {
				char* log = (char*)malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(this->m_shader, logLen, &written, log);
				printf("Shader log:\n%s", log);
				free(log);
			}
		}
	}

	Shader::~Shader() {
		glDeleteShader(this->m_shader);
	}
}
