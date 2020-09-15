#include "Transform.h"
#include <iostream>

namespace Cygnus3D {
		Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) {
			m_position = position;
			m_oldPosition = m_position;
			m_scale = scale;
			m_rotation = rotation;

			m_hasChanged = true;

			updateRotation();
		}


		Transform::~Transform() {

			delete m_parent;

		}

		void Transform::updateRotation() {
			glm::mat4 rotation = glm::mat4_cast(m_rotation);

			m_rotationMatrix = glm::mat4(1.0f);

			m_rotationMatrix = glm::translate(m_rotationMatrix, m_position);
			m_rotationMatrix = rotation * m_rotationMatrix;
			m_rotationMatrix = glm::translate(m_rotationMatrix, -m_position);
			m_hasChanged = true;
		}

		void Transform::setPosition(glm::vec3 position) {
			m_position = position;

			m_hasChanged = true;
		}

		void Transform::setScale(glm::vec3 scale) {
			m_scale = scale;

			m_hasChanged = true;
		}

		void Transform::setRotation(float x, float y, float z, float w) {

			m_rotation.x = x;
			m_rotation.y = y;
			m_rotation.z = z;
			m_rotation.w = w;

			updateRotation();
		}

		void Transform::rotate(const glm::vec3 axis, float angle) {
			
			rotate(glm::angleAxis(glm::radians(angle), axis));
		}

		void Transform::rotate(const glm::quat &rotation) {

			m_rotation = glm::normalize(m_rotation * rotation);

			updateRotation();
		}

		glm::vec3 Transform::rotate(glm::vec3 axis, glm::quat rotation) {

			glm::quat q = glm::conjugate(rotation);
			glm::quat newRotation = rotation * glm::quat(0, axis) * q;

			return glm::normalize(glm::vec3(newRotation[0], newRotation[1], newRotation[2]));

		}

		glm::vec3 Transform::getWorldPosition() {

			glm::vec4 position = m_transform * glm::vec4(m_position, 1.0f);

			return position;
		}

		glm::vec3 Transform::getWorldScale() {

			glm::vec3 scale = glm::vec3(m_transform[0][0], m_transform[1][1], m_transform[2][2]);

			if (scale.x < 0) scale.x *= -1;
			if (scale.y < 0) scale.y *= -1;
			if (scale.z < 0) scale.z *= -1;

			return scale;
		}

		void Transform::updateTransform() {

			if (m_hasChanged) {

				m_transform = glm::mat4(1.0f);	

				if (m_parent && m_position == m_oldPosition && m_scale == m_oldScale && m_rotation == m_oldRotation) {
					m_transform = m_oldParentTransform * m_rotationMatrix;
				}
				else {

					m_transform = { m_scale.x, 0, 0, 0,
									0, m_scale.y, 0, 0,
									0, 0, m_scale.z, 0,
									m_position.x, m_position.y, m_position.z, 1.0f };

					m_transform = m_transform * m_rotationMatrix;

					if (m_parent) {
						m_transform = m_parent->getTransformMatrix() * m_transform;
					}
				}

				m_oldPosition = m_position;
				m_oldScale = m_scale;
				m_oldRotation = m_rotation;
				m_hasChanged = false;
			}
		}

		glm::mat4 Transform::getTransformMatrix() {

			if (m_parent && m_oldParentTransform != m_parent->getTransformMatrix()) {
				m_hasChanged = true;
				m_oldParentTransform = m_parent->getTransformMatrix();
			}else if (m_position != m_oldPosition || m_scale != m_oldScale) {
				m_hasChanged = true;
			}

			if (m_hasChanged) {
				updateTransform();
			}

			return m_transform;
		}

		void Transform::setParent(Transform *parent) {
			m_parent = parent;
		}

		glm::vec3 Transform::getUp() {

			glm::vec3 up = rotate(glm::vec3(0.0f, 1.0f, 0.0f), m_rotation);

			return up;
		}
}