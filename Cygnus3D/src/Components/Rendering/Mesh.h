#pragma once

#include <GLEW/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <vector>
#include <map>
#include "../Animation/Joint.h"

namespace Cygnus3D {

	class Mesh {

	friend class Node;
	friend class Renderer;
	friend class MeshLoader;
	friend class PostProcessing;

	#define POSITION_LOCATION 0
	#define TEXCOORD_LOCATION 1
	#define NORMAL_LOCATION 2
	#define BONES_ID_LOCATION 3
	#define BONES_WEIGHT_LOCATION 4

	#define BUFFERS_NUMBER 5

	private:

		unsigned int vao;
		unsigned int vio;

		unsigned int m_buffers[BUFFERS_NUMBER];

		glm::vec3 m_max, m_min;
		glm::vec3 m_center;

		bool m_hasBones;

	public:

		Mesh(std::vector<glm::vec3> &positions, std::vector<unsigned int> &indices);
		Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<unsigned int> &indices);
		Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<unsigned int> &indices);
		Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals, std::vector<unsigned int> &indices);

		Mesh(const Mesh *mesh);

		void setNormals(std::vector<glm::vec3> &normals);
		void setTexCoords(std::vector<glm::vec2> &texCoords);

		inline glm::vec3 getMax() const { return m_max; }
		inline bool meshHasBones() const { return m_hasBones; }

		void generate();


	protected:

		Mesh() {};

		GLenum m_topology = GL_TRIANGLES;

		std::vector<glm::vec3> m_positions;
		std::vector<glm::vec2> m_texCoords;
		std::vector<glm::vec3> m_normals;

		std::vector<glm::ivec3> m_bonesID;
		std::vector<glm::vec3> m_bonesWeight;

		bool m_faceCulling = false;

		std::vector<unsigned int> m_indices;

	};
}