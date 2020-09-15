#pragma once 

#include "Texture.h"
#include <vector>

namespace Cygnus3D {
	class Cubemap : public Texture {

	public:
		Cubemap(std::vector<const char*> facesPath);

		void bind() override;
		void unbind() override;

	};
}