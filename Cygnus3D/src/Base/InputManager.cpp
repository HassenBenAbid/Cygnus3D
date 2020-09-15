#include "InputManager.h"
#include "../Components/Camera.h"

namespace Cygnus3D {
		std::vector<int> InputManager::keysPressed;
		std::vector<int> InputManager::keysReleased;
		std::vector<int> InputManager::keysHolding;
		std::vector<int> InputManager::buttonsPressed;
		std::vector<int> InputManager::buttonsReleased;
		std::vector<int> InputManager::buttonsHolding;

		glm::dvec2 InputManager::m_cursorPosition(0, 0);

		float InputManager::m_lastXPos;
		float InputManager::m_lastYPos;

		Camera* InputManager::m_mainCamera;

		bool InputManager::m_firstMouse = true;
		bool InputManager::m_inputMode = false;

		void InputManager::initCallbacks(GLFWwindow *window) {
			glfwSetKeyCallback(window, keyCallback);
			glfwSetMouseButtonCallback(window, mouseButtonCallback);
			glfwSetCursorPosCallback(window, cursorPosCallback);

			std::cout << "INPUT CALLBACK INITIALIZED!" << std::endl;
		}

		void InputManager::initCallbacks(GLFWwindow *window, Camera *camera) {

			m_mainCamera = camera;

			glfwSetKeyCallback(window, keyCallback);
			glfwSetMouseButtonCallback(window, mouseButtonCallback);
			glfwSetCursorPosCallback(window, cursorPosCallback);

			std::cout << "INPUT CALLBACK INITIALIZED!" << std::endl;
		}

		void InputManager::update() {
			glfwPollEvents();
		}

		void InputManager::clear() {
			keysPressed.clear();
			keysReleased.clear();
			buttonsPressed.clear();
			buttonsReleased.clear();
		}

		void InputManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, true);
			}

			if (action == GLFW_PRESS) {
				if (std::find(keysPressed.begin(), keysPressed.end(), key) == keysPressed.end()) {
					keysPressed.push_back(key);
					keysHolding.push_back(key);
				}
			}

			if (action == GLFW_RELEASE) {
				for (std::vector<int>::iterator iter = keysHolding.begin(); iter < keysHolding.end(); ++iter) {
					if (*iter == key) {
						keysHolding.erase(iter);
						break;
					}
				}

				keysReleased.push_back(key);
			}

		}

		void InputManager::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

			/* //Hide Cursor on click
			if (button == 1 && action == GLFW_PRESS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				m_cursorLocked = true;
				setLockedCursorPosition(m_cursorPosition);
			}

			if (button == 1 && action == GLFW_RELEASE) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_cursorLocked = false;
				setCursorPosition(window, m_lockedCursorPosition);
			}
			*/

			if (action == GLFW_PRESS) {
				if (std::find(buttonsPressed.begin(), buttonsPressed.end(), button) == buttonsPressed.end()) {
					buttonsPressed.push_back(button);
					buttonsHolding.push_back(button);
				}
			}

			if (action == GLFW_RELEASE) {
				for (std::vector<int>::iterator iter = buttonsHolding.begin(); iter < buttonsHolding.end(); iter++) {
					if (*iter == button) {
						buttonsHolding.erase(iter);
						break;
					}
				}

				buttonsReleased.push_back(button);
			}
		}

		void InputManager::cursorPosCallback(GLFWwindow* window, double x, double y) {

			if (m_mainCamera && m_inputMode) {

				if (m_firstMouse) {
					m_lastXPos = x;
					m_lastYPos = y;
					m_firstMouse = false;
				}

				float xOffset = x - m_lastXPos;
				float yOffset = y - m_lastYPos;

				m_lastXPos = x;
				m_lastYPos = y;

				m_mainCamera->inputMouse(xOffset, yOffset);

				//m_cursorPosition = glm::vec2(x, y);
				//glfwSetCursorPos(window, x, y);
			}
			else {
				m_cursorPosition = glm::vec2(x, y);
				glfwSetCursorPos(window, x, y);
			}



		}

		glm::vec2 InputManager::getCursorPosition() {
			return m_cursorPosition;
		}

		void InputManager::setCursorPosition(GLFWwindow *window, glm::vec2 position) {
			m_cursorPosition = position;
			glfwSetCursorPos(window, position.x, position.y);
		}

		bool InputManager::isKeyPressed(int key) {
			return (std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end());
		}

		bool InputManager::isKeyHolded(int key) {
			return (std::find(keysHolding.begin(), keysHolding.end(), key) != keysHolding.end());
		}

		void InputManager::inputMode(GLFWwindow *window, bool status) {
			if (status) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_firstMouse = true;
			}

			m_inputMode = status;
		}

		bool InputManager::isButtonReleased(int button) {
			return (std::find(buttonsReleased.begin(), buttonsReleased.end(), button) != buttonsReleased.end());
		}


}