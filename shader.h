#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <cassert>

namespace nex {
	class Shader {
	private:
		GLuint m_shader;

	public:
		Shader() {};
		~Shader();
		void create(std::string file, GLuint shaderType);
		GLuint get() { return m_shader; }
		
	};
}

#endif