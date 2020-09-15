#include "Window.h"

namespace Cygnus3D {

		GLFWwindow* Window::m_window;
		int Window::m_width;
		int Window::m_height;

		double Window::m_deltaTime;
		double Window::m_lastTime;

		Window::Window(char const *name, int width, int height) {

			m_name = name;
			m_width = width;
			m_height = height;

			if (!init())
				glfwTerminate();
		}

		Window::~Window() {
			glfwTerminate();
		}

		bool Window::init() {
			if (!glfwInit()) {
				std::cout << "Failed to initialize GLFW!" << std::endl;
				return false;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

			m_window = glfwCreateWindow(m_width, m_height, m_name, NULL, NULL);
			if (!m_window) {
				std::cout << "Failed to create GLFW window!" << std::endl;
				return false;
			}

			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetWindowSizeCallback(m_window, windowResize);
			glfwSwapInterval(0.0);

			if (glewInit() != GLEW_OK) {
				std::cout << "Failed to initialize GLEW!" << std::endl;
				return false;
			}

			m_lastTime = glfwGetTime();
			m_deltaTime = m_lastTime;

			return true;
		}

		bool Window::closed() const{
			return glfwWindowShouldClose(m_window);
		}

		void Window::clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}

		void Window::update(){

			updateDeltaTime();

			glfwPollEvents();
			
			glfwSwapBuffers(m_window);

			GLenum errors = glGetError();

			if (errors != GL_NO_ERROR)
				std::cout << "GL ERROR : " << errors << std::endl;

		}

		GLFWwindow* Window::getWindow() {
			return m_window;
		}

		int Window::getWidth() {
			return m_width;
		}

		int Window::getHeight() {
			return m_height;
		}

		void windowResize(GLFWwindow *window, int width, int height) {
			//glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
		}

		void Window::updateDeltaTime() {

			double currentTime = glfwGetTime();
			m_deltaTime = currentTime - m_lastTime;
			m_lastTime = currentTime;

		}

		void Window::enableMSAA() {
			glfwWindowHint(GLFW_SAMPLES, 4);
		}

}