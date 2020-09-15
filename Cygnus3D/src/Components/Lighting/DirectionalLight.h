#pragma once

#include "Light.h"

namespace Cygnus3D {
	class DirectionalLight : public Light {
	public:

		glm::vec3 direction;
		
	};
}