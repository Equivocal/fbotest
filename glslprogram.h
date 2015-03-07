#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <GL/glew.h>
#include <cassert>
#include <string>

#include "shader.h"

namespace nex {
	class GlslProgram {
	private:
		GLint m_handle;
		Shader m_vShader;
		Shader m_fShader;

	public:
		GLint create(const std::string& vertShader, const std::string& fragShader);
		GLint get();
		void use();
		void destroy();

	};
}

#endif