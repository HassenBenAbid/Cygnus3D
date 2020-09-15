#pragma once

#include "../../shaders/shader.h"

namespace Cygnus3D {

	class Quad;

	class PostProcessing {

		friend class DebugInterface;

	private:

		static Shader *m_shader;
		static Quad *m_screenQuad;
		
		static unsigned int m_fbo, m_intermediateFbo;
		static unsigned int m_screenTexture;
		static unsigned int m_width, m_height;
		static bool m_enabled;

		static void initShader();
		static void initIntermediateBuffer();

		//effects
		static bool m_blackAndWhite;

	public:
		static void init(unsigned int width, unsigned int height);
		
		static void begin();
		static void end();

		static void enable();
		static void disable();

		static void statusBAW(bool status);

	};
}