#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Cygnus3D {

	enum lightTypes {
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	struct Light {

		Light(lightTypes Type);

		glm::vec4 color;

		glm::vec3 position;
		float range;
		
		glm::vec3 direction;
		float intensity;
		
		
		float spotLightAngle = 0;
		bool enabled;
		lightTypes type ;

		float PADDING;
	};
}