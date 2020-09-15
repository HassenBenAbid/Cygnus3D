#pragma once

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Cygnus3D {

		void windowResize(GLFWwindow *window, int width, int height);

		class Window {
		private:
			static GLFWwindow *m_window;
			static int m_width, m_height;
			char const *m_name;
			bool m_closed;

			static double m_deltaTime, m_lastTime;

			bool init();
			static void updateDeltaTime();

		public:
			Window(char const *name, int width, int height);
			~Window();

			bool closed() const;
			void update();
			static void clear();

			static GLFWwindow* getWindow();
			static int getWidth();
			static int getHeight();
			static void enableMSAA();

			inline static double getDeltaTime() { return m_deltaTime; }

		};

}

