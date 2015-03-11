#ifndef FBO_H
#define FBO_H

#define GLM_FORCE_RADIANS 

#include <GL/glew.h>
#include <SDL.h>
#undef main
#include <SDL_opengl.h>
#include <vector>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "primitive.h"
#include "glslprogram.h"

namespace nex {
	class FBO {
	private:
		GLuint m_occluderFBO, m_occluderTex, m_vbo;
		GLuint m_shadowFBO, m_shadowTex;
		GLuint m_polarFBO, m_polarTex;
		GLint m_model[5], m_view[5], m_projection[5];
		GLint m_windowSize[2];

		glm::vec2 m_screenSize;

		nex::GlslProgram m_prog1;
		nex::GlslProgram m_prog2;
		nex::GlslProgram m_prog3;
		nex::GlslProgram m_prog4;
		nex::GlslProgram m_prog5;

		int m_state;

		std::vector<nex::Vertex> verts;
		std::vector<nex::Vertex> verts2;
		std::vector<nex::Vertex> verts3;
		std::vector<nex::Vertex> verts4;

	public:
		void setup();
		void draw();

		int loop();
	};
}


#endif