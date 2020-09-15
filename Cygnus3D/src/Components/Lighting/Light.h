#pragma once

#include <glm/vec3.hpp>

namespace Cygnus3D {
	class Light {

	public:

		static glm::vec3 ambient;

		glm::vec3 diffuse;
		glm::vec3 specular;

	};
}