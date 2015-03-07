#include "glslprogram.h"

namespace nex {
	GLint GlslProgram::create(const std::string& vertShader, const std::string& fragShader) {
		m_vShader.create(vertShader, GL_VERTEX_SHADER);
		m_fShader.create(fragShader, GL_FRAGMENT_SHADER);

		this->m_handle = glCreateProgram();
		if (this->m_handle == 0) {
			printf("Error creating program object");
			exit(1);
		}

		glAttachShader(m_handle, m_vShader.get());
		glAttachShader(m_handle, m_fShader.get());

		glBindAttribLocation(m_handle, 0, "VertexPosition");
		glBindAttribLocation(m_handle, 1, "texCoord");
		//glBindAttribLocation(m_handle, 2, "depthCoord");

		glLinkProgram(m_handle);

		GLint texUnitLoc = glGetUniformLocation(m_handle, "texUnit");
		glActiveTexture(GL_TEXTURE0);
		glProgramUniform1i(m_handle, texUnitLoc, 0);
		assert(glGetError() == GL_NO_ERROR);

		GLint status;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			printf("Failed to link shader program!");
			GLint logLen;
			glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0) {
				char* log = (char*)malloc(logLen);
				GLsizei written;
				glGetProgramInfoLog(m_handle, logLen, &written, log);
				printf("Program log: \n%s", log);
				free(log);
			}
		}
		else {
			glUseProgram(m_handle);
		}
		assert(glGetError() == GL_NO_ERROR);

		return m_handle;
	}

	void GlslProgram::use() {
		glUseProgram(m_handle);
	}

	void GlslProgram::destroy() {
		glDeleteProgram(m_handle);
	}

	GLint GlslProgram::get() {
		return m_handle;
	}
}