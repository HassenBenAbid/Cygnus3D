#pragma once 

#include <glm/vec3.hpp>
#include "../../texture/Texture.h"

namespace Cygnus3D {

	class Node;

	class Material {

		friend class Renderer;

	private:

		Texture *m_texture;
		Texture *m_specularMap;

		glm::vec3 m_color;

		float m_shininess;

	public:

		Material();
		~Material();

		Material(const Material *material);

		void setTexture(Texture *texture);
		void setSpecularMap(Texture *texture);
		void setColor(glm::vec3 color);
		void setShininess(float shininess);

		inline Texture* getTexture() const { return m_texture; }

	};
}