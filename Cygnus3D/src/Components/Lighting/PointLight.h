#pragma once

#include "Light.h"

namespace Cygnus3D {
	class PointLight : public Light {

	public :

		glm::vec3 position;

		float constant;
		float linear;
		float quadratic;

	};

}