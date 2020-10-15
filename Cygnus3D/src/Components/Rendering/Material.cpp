#include "Material.h"

namespace Cygnus3D {

	Material::Material(){
		diffuseColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		emissiveColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		opacity = 1.0f;
		bumpIntensity = 0;
		alphaThreshold = 0.1f;

		specularPower = 20.0f;
		specularScale = 0;

		m_DiffuseTextureState = false;
		m_SpecularTextureState = false;
		m_normalTextureState = false;
		m_opacityTextureState = false;
		m_emissiveTextureState = false;
	}

	Material::~Material() {
		delete m_diffuseTexture;
		delete m_specularTexture;
		delete m_normalTexture;
		delete m_opacityTexture;
		delete m_emissiveTexture;
	}	

	Material::Material(const Material *material) {
		diffuseColor = material->diffuseColor;
		specularColor = material->specularColor;
		emissiveColor = material->emissiveColor;

		opacity = material->opacity;
		bumpIntensity = material->bumpIntensity;
		alphaThreshold = material->alphaThreshold;

		specularScale = specularScale;
		specularPower = material->specularPower;

		setDiffuseTexture(material->m_diffuseTexture);
		setSpecularTexture(material->m_specularTexture);
		setNormalTexture(material->m_diffuseTexture);
		setOpacityTexture(material->m_opacityTexture);
		setEmissiveTexture(material->m_emissiveTexture);
	}

	void Material::setDiffuseTexture(Texture *diffuse) {
		if (diffuse != nullptr) {
			m_diffuseTexture = diffuse;
			m_DiffuseTextureState = true;
		}
	}

	void Material::setSpecularTexture(Texture *specular) {
		if (specular != nullptr) {
			m_specularTexture = specular;
			m_SpecularTextureState = true;
		}
	}

	void Material::setNormalTexture(Texture *normal) {
		if (normal != nullptr) {
			m_normalTexture = normal;
			m_normalTextureState = true;
		}
	}

	void Material::setOpacityTexture(Texture *opa) {
		if (opa != nullptr) {
			m_opacityTexture = opa;
			m_opacityTextureState = true;
		}
	}

	void Material::setEmissiveTexture(Texture *emissive) {
		if (emissive != nullptr) {
			m_emissiveTexture = emissive;
			m_emissiveTextureState = true;
		}
	}

}