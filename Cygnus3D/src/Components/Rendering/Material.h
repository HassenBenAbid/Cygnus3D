#pragma once 

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../../texture/Texture.h"

namespace Cygnus3D {

	class Node;

	class Material {

	private:

		Texture *m_diffuseTexture;
		Texture *m_specularTexture;
		Texture *m_normalTexture;
		Texture *m_opacityTexture;
		Texture *m_emissiveTexture;

		bool m_DiffuseTextureState;
		bool m_SpecularTextureState;
		bool m_normalTextureState;
		bool m_opacityTextureState;
		bool m_emissiveTextureState;

	public:

		Material();
		~Material();

		Material(const Material *material);

		glm::vec4 diffuseColor;
		glm::vec4 specularColor;
		glm::vec4 emissiveColor;

		float opacity;
		float specularPower;
		float specularScale;
		float alphaThreshold;
		float bumpIntensity;


		void setDiffuseTexture(Texture *diffuse);
		void setSpecularTexture(Texture *specular);
		void setNormalTexture(Texture *normal);
		void setOpacityTexture(Texture *opa);
		void setEmissiveTexture(Texture *emissive);

		bool hasDiffuseTexture() const { return m_DiffuseTextureState; }
		Texture* getDiffuseTexture() const { return m_diffuseTexture; }

		bool hasSpecularTexture() const { return m_SpecularTextureState; }
		Texture* getSpecularTexture() const { return m_specularTexture; }

		bool hasNormalTexture() const { return m_normalTextureState; }
		Texture* getNormalTexture() const { return m_normalTexture; }

		bool hasOpacityTexture() const { return m_opacityTextureState; }
		Texture* getOpacityTexture() const { return m_opacityTexture; }

		bool hasEmissiveTexture() const { return m_emissiveTextureState; }
		Texture* getEmissiveTexture() const { return m_emissiveTexture; }

	};
}