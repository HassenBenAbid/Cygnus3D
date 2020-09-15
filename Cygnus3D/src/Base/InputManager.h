#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

namespace Cygnus3D {

		class Camera;

		class InputManager {

		private:
			static std::vector<int> keysPressed;
			static std::vector<int> keysReleased;
			static std::vector<int> keysHolding;
			static std::vector<int> buttonsPressed;
			static std::vector<int> buttonsReleased;
			static std::vector<int> buttonsHolding;


			static glm::dvec2 m_cursorPosition;
			static bool m_firstMouse;
			static bool m_inputMode;

			static float m_lastXPos, m_lastYPos;

			static Camera *m_mainCamera;

			static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
			static void cursorPosCallback(GLFWwindow *window, double x, double y);

		public:

			static void update();
			static void initCallbacks(GLFWwindow *window);
			static void initCallbacks(GLFWwindow *window, Camera *camera);

			static void clear();

			//CURSOR POSITION
			static glm::vec2 getCursorPosition();
			static void setCursorPosition(GLFWwindow *window, glm::vec2 position);
			static void inputMode(GLFWwindow *window, bool status);

			//KEYS STATUS
			static bool isKeyPressed(int key);
			static bool isKeyHolded(int key);

			//BUTTON STATUS
			static bool isButtonReleased(int button);

			//
			//static void cameraRotation(glm::vec3 *target);


		};
}