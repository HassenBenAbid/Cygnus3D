#include "Joint.h"
#include <iostream>

namespace Cygnus3D {

	Joint::Joint(std::string name) : m_name(name){

		m_currentFrame = 0;
		m_nextFrame = 1;
		m_timer = 0;

	}

	void Joint::addChild(Joint *joint) {
		m_children.push_back(joint);
	}

	void Joint::addTransform(jointTransform* transform) {
		m_jointTransforms.push_back(transform);
	}

	glm::mat4 Joint::getJointTransform(float deltaTime, unsigned int animationIndex) {

		m_timer += deltaTime;
		getCurrentFrames(m_timer, animationIndex);

		float factor = getProgression(m_jointTransforms[animationIndex]->timeDuration[m_currentFrame], m_jointTransforms[animationIndex]->timeDuration[m_nextFrame], m_timer);

		glm::vec3 startPos = m_jointTransforms[animationIndex]->positions[m_currentFrame];
		glm::vec3 endPos = m_jointTransforms[animationIndex]->positions[m_nextFrame];
		glm::vec3 newPosition = interpolateVector(startPos, endPos, factor);
		glm::mat4 positionMatrix = glm::mat4(1.0f);
		positionMatrix = glm::translate(positionMatrix, newPosition);

		glm::quat startRot = m_jointTransforms[animationIndex]->rotations[m_currentFrame];
		glm::quat endRot = m_jointTransforms[animationIndex]->rotations[m_nextFrame];
		glm::quat newRotation = glm::slerp(startRot, endRot, factor);
		glm::mat4 rotationMatrix = glm::mat4_cast(newRotation);

		return positionMatrix * rotationMatrix;

	}

	void Joint::getCurrentFrames(float animationTime, unsigned int animationIndex) {

		if (animationTime >= m_jointTransforms[animationIndex]->timeDuration[m_nextFrame]) {

			if (m_nextFrame == m_jointTransforms[animationIndex]->timeDuration.size() - 1) {
				m_currentFrame = 0;
				m_nextFrame = 1;
				m_timer = 0;
			}
			else {
				m_currentFrame++;
				m_nextFrame++;
			}

		}

	}

	float Joint::getProgression(float previousFrame, float nextFrame, float animationTime) {

		float totalTime = nextFrame - previousFrame;
		float currentTime = animationTime - previousFrame;

		return currentTime / totalTime;
	}

	glm::vec3 Joint::interpolateVector(glm::vec3 start, glm::vec3 end, float progression) {

		glm::vec3 delta = end - start;

		return start + delta * progression;

	}

}