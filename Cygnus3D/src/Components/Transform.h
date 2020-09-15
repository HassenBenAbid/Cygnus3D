#pragma once 

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Cygnus3D {
		class Transform {

			friend class Physics;

		private:

			glm::vec3 m_position, m_oldPosition;
			glm::vec3 m_scale, m_oldScale;

			glm::mat4 m_rotationMatrix;
			glm::quat m_rotation, m_oldRotation;

			glm::mat4 m_transform;
			glm::mat4 m_oldParentTransform;

			Transform *m_parent;

			void updateRotation();
			void updateTransform();

			glm::vec3 rotate(glm::vec3 axis, glm::quat rotation);

		public:

			Transform(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::quat& rotation = glm::quat(0, 0, 0, 1.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
			virtual ~Transform();

			virtual void setPosition(glm::vec3 position);
			virtual void setScale(glm::vec3 scale);
			virtual void setRotation(float x, float y, float z, float w);

			virtual void rotate(const glm::vec3 axis, float angle);
			void rotate(const glm::quat &rotation);

			glm::vec3 getWorldPosition();
			glm::vec3 getWorldScale();

			inline glm::vec3& getLocalPosition() { return m_position; }
			inline glm::vec3& getLocalScale() { return m_scale; }

			inline glm::mat4 getRotationMatrix() const { return m_rotationMatrix; }
			inline glm::quat getRotation() const { return m_rotation; }

			glm::mat4 getTransformMatrix();

			glm::vec3 getUp();

		protected:

			bool m_hasChanged;

			inline glm::mat4 getCurrentTransform() const { return m_transform; }
			void setParent(Transform *parent);

		};
}