#include "Animator.h"
#include <iostream>

namespace Cygnus3D {

	Animator::Animator(Joint *rootJoint) : m_rootJoint(rootJoint) {
		m_currentAnimation = 0;
	}

	void Animator::update(float deltaTime, glm::mat4 inverseNode) {
		glm::mat4 parentMatrix = glm::mat4(1.0f);
		updateJointTransform(deltaTime, m_rootJoint, parentMatrix, inverseNode);
	}

	void Animator::updateJointTransform(float DeltaTime, Joint *joint, glm::mat4 parentMatrix, glm::mat4 inverseNode) {

		if (m_animate) {
			glm::mat4 globalTransformation = glm::mat4(1.f);

			if (m_boneMapping.find(joint->m_name) != m_boneMapping.end()) {

				globalTransformation = parentMatrix * joint->getJointTransform(DeltaTime, m_currentAnimation);

				unsigned int boneIndex = m_boneMapping[joint->m_name];
				m_boneInfo[boneIndex]->finalTransformation = globalTransformation * m_boneInfo[boneIndex]->boneOffset;

			}
			else {
				globalTransformation = parentMatrix;
			}

			for (int i = 0; i < joint->m_children.size(); i++) {
				updateJointTransform(DeltaTime, joint->m_children[i], globalTransformation, inverseNode);
			}
		}

	}

	void Animator::setAnimation(unsigned int animation) {

		if (animation < m_animationCount) m_currentAnimation = animation;

	}

	void Animator::start(bool state) {
		m_animate = state;
	}

	std::vector<glm::mat4> Animator::getBonesTransformation() {

		std::vector<glm::mat4> transformations;

		for (int i = 0; i < m_boneInfo.size(); i++) {
			transformations.push_back(m_boneInfo[i]->finalTransformation);
		}

		return transformations;
	}

}