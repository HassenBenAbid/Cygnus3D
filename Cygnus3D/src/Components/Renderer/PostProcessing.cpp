#include "PostProcessing.h"
#include "../Rendering/Quad.h"

namespace Cygnus3D {

	Shader* PostProcessing::m_shader;
	Quad* PostProcessing::m_screenQuad;

	unsigned int PostProcessing::m_fbo;
	unsigned int PostProcessing::m_intermediateFbo;
	unsigned int PostProcessing::m_screenTexture;
	unsigned int PostProcessing::m_width;
	unsigned int PostProcessing::m_height;
	bool PostProcessing::m_enabled = false;

	bool PostProcessing::m_blackAndWhite = false;

	void PostProcessing::initShader() {
		m_shader = new Shader("resources/shaders/postprocessing.vert", "resources/shaders/postprocessing.frag");

		m_shader->enable();
		m_shader->setUniform1i("screenTexture", m_screenTexture);
		m_shader->disable();
	}

	void PostProcessing::initIntermediateBuffer() {

		glGenFramebuffers(1, &m_intermediateFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_intermediateFbo);

		glGenTextures(1, &m_screenTexture);
		glBindTexture(GL_TEXTURE_2D, m_screenTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "INTERMEDIATE FRAMEBUFFER ERROR!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PostProcessing::init(unsigned int width, unsigned int height) {

		unsigned int sampledTexture;
		m_width = width;
		m_height = height;

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		glGenTextures(1, &sampledTexture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, sampledTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, sampledTexture, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "FRAMEBUFFER ERROR!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		initIntermediateBuffer();
		initShader();
		m_screenQuad = new Quad(2.0f);
	}

	void PostProcessing::begin() {
		if (m_enabled) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
	}

	void PostProcessing::end() {
		if (m_enabled) {

			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediateFbo);
			glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

			m_shader->enable();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glActiveTexture(GL_TEXTURE0 + m_screenTexture);
			glBindTexture(GL_TEXTURE_2D, m_screenTexture);

			glBindVertexArray(m_screenQuad->vao);
			glDrawElements(m_screenQuad->m_topology, m_screenQuad->m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void PostProcessing::enable() {
		m_enabled = true;
	}

	void PostProcessing::disable() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_enabled = false;
	}

	void PostProcessing::statusBAW(bool status) {
		m_blackAndWhite = status;
		m_shader->setUniform1i("blackAndWhite", m_blackAndWhite);
	}
}