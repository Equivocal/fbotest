#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "primitive.h"
#include "shader.h"

GLuint m_occluderFBO, m_occluderTex, m_vbo;
GLint m_handle, m_model, m_view, m_projection;

nex::Shader m_vShader;
nex::Shader m_fShader;

std::vector<nex::Vertex> verts;

void setup();
void draw();

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	setup();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void setup() {
	GLint err = glewInit();

	glBindFramebuffer(GL_FRAMEBUFFER, m_occluderFBO);
	glGenTextures(1, &m_occluderTex);
	glBindTexture(GL_TEXTURE_2D, m_occluderTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 256);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 256);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_occluderTex, 0);


	m_vShader.create("passthrough.vert", GL_VERTEX_SHADER);
	m_fShader.create("passthrough.frag", GL_FRAGMENT_SHADER);

	m_handle = glCreateProgram();
	if (m_handle == 0) {
		printf("Error creating program object");
		exit(1);
	}

	glAttachShader(m_handle, m_vShader.get());
	glAttachShader(m_handle, m_fShader.get());

	glBindAttribLocation(m_handle, 0, "vertexPosition");
	glBindAttribLocation(m_handle, 1, "texCoord");

	glLinkProgram(m_handle);

	GLint texUnitLoc = glGetUniformLocation(m_handle, "texUnit");
	glActiveTexture(GL_TEXTURE0);
	glProgramUniform1i(m_handle, texUnitLoc, 0);

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
	
	GLuint vboHandles[1];
	glGenBuffers(1, vboHandles);
	m_vbo = vboHandles[0];

	m_view = glGetUniformLocation(m_handle, "view");
	m_model = glGetUniformLocation(m_handle, "model");
	m_projection = glGetUniformLocation(m_handle, "projection");

	verts.push_back({ glm::vec2(100.0, 150.0), glm::vec2(0.0, 1.0), 0.0f, -1, nex::GEO_DEFAULT });
	verts.push_back({ glm::vec2(150.0, 150.0), glm::vec2(1.0, 1.0), 0.0f, -1, nex::GEO_DEFAULT });
	verts.push_back({ glm::vec2(150.0, 100.0), glm::vec2(1.0, 0.0), 0.0f, -1, nex::GEO_DEFAULT });
	verts.push_back({ glm::vec2(100.0, 100.0), glm::vec2(0.0, 0.0), 0.0f, -1, nex::GEO_DEFAULT });

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts.size(), &verts[0], GL_STATIC_DRAW);

	glUniformMatrix4fv(m_projection, 1, false, glm::value_ptr(glm::ortho(0.0f, 640.0f, 480.0f, 0.0f)));
	glUniformMatrix4fv(m_model, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(m_view, 1, false, glm::value_ptr(glm::mat4(1.0f)));
}

void draw() {
	

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	

	// define the index array for the outputs
	

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(nex::Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(nex::Vertex), reinterpret_cast<void*>(sizeof(glm::vec2)));

	glBindFramebuffer(GL_FRAMEBUFFER, m_occluderFBO);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glDrawArrays(GL_QUADS, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_occluderTex);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_QUADS, 0, 4);
}
