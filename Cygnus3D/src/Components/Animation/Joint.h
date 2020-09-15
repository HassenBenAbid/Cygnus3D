#pragma once

#include <string>
#include <vector>
#include <map>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Cygnus3D {

	struct jointTransform {

		std::vector<glm::vec3> positions;
		std::vector<glm::quat> rotations;
		std::vector<float> timeDuration;

	};

	class Joint {

		friend class Animator;
		friend class MeshLoader;

	private:

		std::string m_name;
		std::vector<Joint*> m_children;

		unsigned int m_currentFrame;
		unsigned int m_nextFrame;

		float m_timer;

		std::vector<jointTransform*> m_jointTransforms;

		void getCurrentFrames(float animationTime, unsigned int animationIndex);
		float getProgression(float previousFrame, float nextFrame, float animationTime);

		glm::vec3 interpolateVector(glm::vec3 start, glm::vec3 end, float progression);

	public:

		Joint(std::string name);

		void addChild(Joint *joint);

		void addTransform(jointTransform* transform);
		glm::mat4 getJointTransform(float deltaTime, unsigned int animationIndex);

	};

}