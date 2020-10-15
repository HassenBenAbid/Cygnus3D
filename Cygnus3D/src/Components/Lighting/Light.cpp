#include "Light.h"

namespace Cygnus3D {

	Light::Light(lightTypes lightType) {
		type = lightType;
		enabled = true;
		intensity = 1.0f;
	}
}