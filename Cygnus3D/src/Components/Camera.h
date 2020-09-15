#pragma once

#include "Transform.h"

namespace Cygnus3D {
		class Camera : public Transform{
		private:

			glm::mat4 m_viewMatrix;
			glm::mat4 m_projectionMatrix;

			float m_sensitivity;

			float m_fieldOfView;
			float m_aspect;
			float m_zNear, m_zFar;

			float m_yaw, m_pitch;
			float m_targetYaw, m_targetPitch;

			glm::vec3 m_forward, m_up, m_position;

		public:

			Camera(float fieldOfView, float aspect, float zNear, float zFar);
			~Camera();

			glm::mat4 getViewMatrix();
			glm::mat4 getProjectionMatrix() const;
			glm::vec3 getCameraForward();

			void updateView();

			void input(float deltaTime);
			void update(float deltaTime);

			void setPosition(glm::vec3 position) override;
			void inputMouse(float deltaX, float deltaY);

		};
}
