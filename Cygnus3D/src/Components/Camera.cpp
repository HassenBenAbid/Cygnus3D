#include "Camera.h"
#include "../Base/InputManager.h"
#include "../utils/Utils.h"

namespace Cygnus3D {
		Camera::Camera(float fieldOfView, float aspect, float zNear, float zFar) :
			m_fieldOfView(fieldOfView), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar) {

			m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspect, m_zNear, m_zFar);
			m_sensitivity = 0.1f;

			m_targetYaw = -190.f;

			m_up = glm::vec3(0.0f, -1.0f, 0.0f);
			m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

			updateView();

		}

		Camera::~Camera() {

		}

		void Camera::input(float deltaTime) {

			if (InputManager::isKeyPressed(GLFW_KEY_W) || InputManager::isKeyHolded(GLFW_KEY_W)) {
				m_position += deltaTime * m_forward;
			}

			if (InputManager::isKeyPressed(GLFW_KEY_S) || InputManager::isKeyHolded(GLFW_KEY_S)) {
				m_position -= deltaTime * m_forward;
			}

			if (InputManager::isKeyPressed(GLFW_KEY_A) || InputManager::isKeyHolded(GLFW_KEY_A)) {
				m_position += glm::normalize(glm::cross(m_forward, m_up)) * deltaTime;
			}

			if (InputManager::isKeyPressed(GLFW_KEY_D) || InputManager::isKeyHolded(GLFW_KEY_D)) {
				m_position -= glm::normalize(glm::cross(m_forward, m_up)) * deltaTime;
			}
		}

		void Camera::update(float deltaTime) {

			m_yaw = lerp(m_yaw, m_targetYaw, (deltaTime * 35.0f));
			m_pitch = lerp(m_pitch, m_targetPitch, (deltaTime * 35.0f));

			//m_yaw = m_targetYaw * 1.5f;
			//m_pitch = m_targetPitch * 1.5f;

			glm::vec3 newForward;

			newForward.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
			newForward.y = -sin(glm::radians(m_pitch));
			newForward.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));

			m_forward = glm::normalize(newForward);
			glm::vec3 right = glm::normalize(glm::cross(m_forward, getUp()));
			m_up = glm::cross(right, m_forward);

			updateView();
		}

		void Camera::updateView() {
			m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, -m_up);
		}

		glm::mat4 Camera::getViewMatrix(){

			return m_viewMatrix;
		}
		
		glm::mat4 Camera::getProjectionMatrix() const {
			return m_projectionMatrix;
		}

		glm::vec3 Camera::getCameraForward() {

			return m_forward;
		}

		void Camera::inputMouse(float deltaX, float deltaY) {

			float xMov = deltaX * m_sensitivity;
			float yMov = deltaY * m_sensitivity;

			m_targetYaw += xMov;
			m_targetPitch += yMov;

			if (m_targetYaw == 0.0f) {
				m_targetYaw = 0.01f;
			}

			if (m_targetPitch == 0.0f) {
				m_targetPitch = 0.01f;
			}

			if (m_targetPitch > 89.0f) {
				m_targetPitch = 89.0f;
			}

			if (m_targetPitch < -89.0f) {
				m_targetPitch = -89.0f;
			}
		}

		void Camera::setPosition(glm::vec3 position) {
			Transform::setPosition(position);

			m_position = position;

			updateView();
		}
}