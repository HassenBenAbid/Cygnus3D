#pragma once

namespace Cygnus3D {
	class Texture {

	protected:

		unsigned int m_textureID;
		unsigned char* m_data;

		int m_width, m_height;
		int m_nbrChannels;

	public:

		Texture() {};
		Texture(const char *filePath);
		~Texture();

		virtual void bind();
		virtual void unbind();

		inline unsigned int getTextureID() const { return m_textureID; }


	};
}