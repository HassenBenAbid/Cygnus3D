#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

namespace Cygnus3D {

	class Node;

	class DebugInterface {

	private :

		ImGuiID m_coreMenu;
		Node *m_focusedNode;

		double m_fpsTimer = 0.0;
		int m_fpsCounter = 0;
		int m_fps = 0;

		void showPerformance();
		void showObjectTransformations();

		void gameobjectWindow();
		void postprocessingWindow();

	public:

		DebugInterface();

		void init(GLFWwindow *window);
		void startUpdate();
		void endUpdate();
		void clean();
		void render();

		void enableInterfaceTransformation(Node *node);
		bool mouseInsideWidget();

	};
}