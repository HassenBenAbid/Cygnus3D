#include "Material.h"

namespace Cygnus3D {

	Material::Material(){
		m_color = glm::vec3(1.0f, 1.0f, 1.0);
	}

	Material::~Material() {
		delete m_texture;
		delete m_specularMap;
	}

	Material::Material(const Material *material) {
		m_color = material->m_color;
		m_shininess = material->m_shininess;

		m_texture = material->m_texture;
		m_specularMap = material->m_specularMap;
	}

	void Material::setColor(glm::vec3 color) {
		m_color = color;
	}

	void Material::setTexture(Texture *texture) {
		m_texture = texture;
	}

	void Material::setSpecularMap(Texture *texture) {
		m_specularMap = texture;
	}

	void Material::setShininess(float shininess) {
		m_shininess = shininess;
	}
}