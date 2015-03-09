#include "fbo.h"
namespace nex {
	int FBO::loop() {
		m_state = 0;
		SDL_Init(SDL_INIT_VIDEO);
		/*
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
			SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);*/

		SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100, 256, 256, SDL_WINDOW_OPENGL);
		SDL_GLContext context = SDL_GL_CreateContext(win);
		assert(glGetError() == GL_NO_ERROR);

		setup();

		SDL_Event event;
		/* Loop until the user closes the window */
		while (1) {
			while (SDL_PollEvent(&event))
			{
				switch (event.type) {
				case SDL_QUIT:
					SDL_Quit();
					return 1;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case SDLK_1:
						m_state = 0;
						break;
					case SDLK_2:
						m_state = 1; break;
					case SDLK_3:
						m_state = 2; break;
					case SDLK_4:
						m_state = 3; break;
					case SDLK_5:
						m_state = 4; break;
					}
				}
			}
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			/* Render here */
			draw();

			SDL_GL_SwapWindow(win);
		}

		return 0;
	}

	void FBO::setup() {
		glewExperimental = GL_TRUE;
		GLint err = glewInit();
		printf("%i\n", glGetError());
		if (err != GL_NO_ERROR) {
			printf("%s\n", glewGetErrorString(err));
			exit(1);
		}
		assert(glGetError() == GL_NO_ERROR);

		glGenFramebuffers(1, &m_occluderFBO);
		glGenFramebuffers(1, &m_shadowFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, m_occluderFBO);
		glGenTextures(1, &m_occluderTex);
		glBindTexture(GL_TEXTURE_2D, m_occluderTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		assert(glGetError() == GL_NO_ERROR);
		/*	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 256);
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 256);
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);*/
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_occluderTex, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*glBindFramebuffer(GL_FRAMEBUFFER, m_polarFBO);
		glGenTextures(1, &m_polarTex);
		glBindTexture(GL_TEXTURE_2D, m_polarTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		assert(glGetError() == GL_NO_ERROR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_polarTex, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
		glGenTextures(1, &m_shadowTex);
		glBindTexture(GL_TEXTURE_2D, m_shadowTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		assert(glGetError() == GL_NO_ERROR);
		/*	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, 256);
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, 256);
			glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);*/
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_shadowTex, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_prog1.create("passthrough.vert", "passthrough.frag");
		m_prog2.create("passthrough.vert", "polar2_debug.frag");
		m_prog3.create("passthrough.vert", "polar2.frag");
		m_prog4.create("passthrough.vert", "shadows2.frag");
		m_prog5.create("passthrough.vert", "green.frag");

		m_view[0] = glGetUniformLocation(m_prog1.get(), "view");
		m_model[0] = glGetUniformLocation(m_prog1.get(), "model");
		m_projection[0] = glGetUniformLocation(m_prog1.get(), "projection");

		m_view[1] = glGetUniformLocation(m_prog2.get(), "view");
		m_model[1] = glGetUniformLocation(m_prog2.get(), "model");
		m_projection[1] = glGetUniformLocation(m_prog2.get(), "projection");
		m_windowSize[0] = glGetUniformLocation(m_prog3.get(), "winsize");

		m_view[2] = glGetUniformLocation(m_prog3.get(), "view");
		m_model[2] = glGetUniformLocation(m_prog3.get(), "model");
		m_projection[2] = glGetUniformLocation(m_prog3.get(), "projection");
		m_windowSize[1] = glGetUniformLocation(m_prog3.get(), "winsize");

		m_view[3] = glGetUniformLocation(m_prog4.get(), "view");
		m_model[3] = glGetUniformLocation(m_prog4.get(), "model");
		m_projection[3] = glGetUniformLocation(m_prog4.get(), "projection");

		m_view[4] = glGetUniformLocation(m_prog5.get(), "view");
		m_model[4] = glGetUniformLocation(m_prog5.get(), "model");
		m_projection[4] = glGetUniformLocation(m_prog5.get(), "projection");

		printf("Vendor: %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version: %s\n", glGetString(GL_VERSION));
		printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		GLuint vboHandles[1];
		glGenBuffers(1, vboHandles);
		m_vbo = vboHandles[0];

		glm::vec2 screenSize = glm::vec2(256.0, 256.0);
		glm::vec2 lightPos = glm::vec2(128.0, 128.0);
		glm::vec2 occluderPos = glm::vec2(80.0, 80.0);
		glm::vec2 occluderPos2 = glm::vec2(256.0 - 100.0, 256.0 - 100.0);
		glm::vec2 occluderSize = glm::vec2(20.0, 20.0);

		glProgramUniformMatrix4fv(m_prog1.get(), m_projection[0], 1, false, glm::value_ptr(glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f)));
		glProgramUniformMatrix4fv(m_prog1.get(), m_model[0], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniformMatrix4fv(m_prog1.get(), m_view[0], 1, false, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))));

		glProgramUniformMatrix4fv(m_prog3.get(), m_projection[2], 1, false, glm::value_ptr(glm::ortho(0.0f, 256.0f, 1.0f, 0.0f)));
		glProgramUniformMatrix4fv(m_prog3.get(), m_model[2], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniformMatrix4fv(m_prog3.get(), m_view[2], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniform2fv(m_prog3.get(), m_windowSize[1], 1, glm::value_ptr(screenSize));

		glProgramUniformMatrix4fv(m_prog2.get(), m_projection[1], 1, false, glm::value_ptr(glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f)));
		glProgramUniformMatrix4fv(m_prog2.get(), m_model[1], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniformMatrix4fv(m_prog2.get(), m_view[1], 1, false, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))));
		glProgramUniform2fv(m_prog2.get(), m_windowSize[0], 1, glm::value_ptr(screenSize));

		glProgramUniformMatrix4fv(m_prog4.get(), m_projection[3], 1, false, glm::value_ptr(glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f)));
		glProgramUniformMatrix4fv(m_prog4.get(), m_model[3], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniformMatrix4fv(m_prog4.get(), m_view[3], 1, false, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))));

		glProgramUniformMatrix4fv(m_prog5.get(), m_projection[4], 1, false, glm::value_ptr(glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f)));
		glProgramUniformMatrix4fv(m_prog5.get(), m_model[4], 1, false, glm::value_ptr(glm::mat4(1.0f)));
		glProgramUniformMatrix4fv(m_prog5.get(), m_view[4], 1, false, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))));

		verts.push_back({ glm::vec2(occluderPos.x, occluderPos.y + occluderSize.y), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos.x + occluderSize.x, occluderPos.y + occluderSize.y), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos.x + occluderSize.x, occluderPos.y), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos.x, occluderPos.y), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos2.x, occluderPos2.y + occluderSize.y), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos2.x + occluderSize.x, occluderPos2.y + occluderSize.y), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos2.x + occluderSize.x, occluderPos2.y), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts.push_back({ glm::vec2(occluderPos2.x, occluderPos2.y), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		/*
			verts2.push_back({ glm::vec2(320.0-128.0, 240.0+128.0), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
			verts2.push_back({ glm::vec2(320.0+128.0, 240.0+128.0), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
			verts2.push_back({ glm::vec2(320.0+128.0, 240.0-128.0), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
			verts2.push_back({ glm::vec2(320.0-128.0, 240.0-128.0), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
			*/
		verts2.push_back({ glm::vec2(0.0, 256.0), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts2.push_back({ glm::vec2(256.0, 256.0), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts2.push_back({ glm::vec2(256.0, 0.0), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts2.push_back({ glm::vec2(0.0, 0.0), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });

		verts4.push_back({ glm::vec2(0.0, 256.0), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts4.push_back({ glm::vec2(256.0, 256.0), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts4.push_back({ glm::vec2(256.0, 246.0), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts4.push_back({ glm::vec2(0.0, 246.0), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });

		verts3.push_back({ glm::vec2(lightPos.x - 128.0f, lightPos.y + 128.0f), glm::vec2(0.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts3.push_back({ glm::vec2(lightPos.x + 128.0f, lightPos.y + 128.0f), glm::vec2(1.0, 1.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts3.push_back({ glm::vec2(lightPos.x + 128.0f, lightPos.y - 128.0f), glm::vec2(1.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
		verts3.push_back({ glm::vec2(lightPos.x - 128.0f, lightPos.y - 128.0f), glm::vec2(0.0, 0.0), 0.0f, 0, nex::GEO_DEFAULT });
	}

	void FBO::draw() {

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts.size(), &verts[0], GL_STREAM_DRAW);

		// define the index array for the outputs
		m_prog1.use();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(nex::Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(nex::Vertex), reinterpret_cast<void*>(sizeof(glm::vec2)));

		if (m_state > 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_occluderFBO);
		}
		GLuint attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_QUADS, 0, 8);

		if (m_state == 0)
			return;

		//// TEMP
		if (m_state == 1) {
			m_prog2.use();

			glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts2.size(), &verts2[0], GL_STREAM_DRAW);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_occluderTex);
			glDrawArrays(GL_QUADS, 0, 4);
			return;
		}
		//// TEMP

		m_prog3.use();

		if (m_state == 2) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, m_shadowFBO);
		}

		glDrawBuffers(1, attachments);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts2.size(), &verts2[0], GL_STREAM_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_occluderTex);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_QUADS, 0, 4);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (m_state == 2)
			return;

		glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts3.size(), &verts3[0], GL_STREAM_DRAW);

		m_prog4.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_shadowTex);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_QUADS, 0, 4);

		glBufferData(GL_ARRAY_BUFFER, sizeof(nex::Vertex)*verts.size(), &verts[0], GL_STREAM_DRAW);

		m_prog5.use();
		//glClearColor(0.0, 0.0, 0.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_QUADS, 0, 8);

	}
}