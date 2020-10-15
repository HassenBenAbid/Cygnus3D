#pragma once

#include <vector>
#include <map>
#include <glm/matrix.hpp>

#include "../Animation/Joint.h"

#define MAX_BONES 100

namespace Cygnus3D {

	class Mesh;

	struct BoneInfo
	{
		glm::mat4 boneOffset;
		glm::mat4 finalTransformation;

		BoneInfo()
		{
			boneOffset = glm::mat4(0);
			finalTransformation = glm::mat4(0);
		}

	};

	class Animator {

		friend class MeshLoader;

	private:

		Joint *m_rootJoint;

		std::map<std::string, unsigned int> m_boneMapping;
		std::vector<BoneInfo*> m_boneInfo;
		std::vector<int> m_ticksPerSecond;

		bool m_animate = true;

		unsigned int m_animationCount;
		unsigned int m_currentAnimation;



		Animator() {};

		void updateJointTransform(float DeltaTime, Joint *joint, glm::mat4 parentMatrix);
		void updateResetJoint(Joint *joint);

	public:

		Animator(Joint *rootJoint);

		void start(bool state);
		void setAnimation(unsigned int animation);
		void resetBindPose();

		bool isActive() const { return m_animate; }

		std::vector<glm::mat4>  getBonesTransformation();

		void update(float deltaTime);

		glm::mat4 m_inverseGlobal;
	};
}