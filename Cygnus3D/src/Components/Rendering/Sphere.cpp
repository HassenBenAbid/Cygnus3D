#include "Sphere.h"
#include "../../utils/Utils.h"

namespace Cygnus3D {
	Sphere::Sphere(unsigned int xSegments, unsigned int ySegments) {

		for (unsigned int y = 0; y <= ySegments; y++) {
			for (unsigned int x = 0; x <= xSegments; x++) {
				float xSegment = (float)x / (float)xSegments;
				float ySegment = (float)y / (float)ySegments;

				float xPos = std::cos(xSegment * (2 * PI)) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * (2 * PI)) * std::sin(ySegment * PI);

				m_positions.push_back(glm::vec3(xPos, yPos, zPos));
				m_texCoords.push_back(glm::vec2(xSegment, ySegment));
				m_normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		for (unsigned int y = 0; y < ySegments; y++) {
			for (unsigned int x = 0; x < xSegments; x++) {
				m_indices.push_back((y + 1) * (xSegments + 1) + x);
				m_indices.push_back(y * (xSegments + 1) + x);
				m_indices.push_back(y * (xSegments + 1) + x + 1);

				m_indices.push_back((y + 1) * (xSegments + 1) + x);
				m_indices.push_back(y * (xSegments + 1) + x + 1);
				m_indices.push_back((y + 1) * (xSegments + 1) + x + 1);
			}
		}

		m_topology = GL_TRIANGLES;
		generate();
	}
}