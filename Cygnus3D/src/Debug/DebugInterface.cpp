#include "DebugInterface.h"

#include "../Base/Window.h"
#include "../Components/Scene/Node.h"
#include "../Components/Renderer/PostProcessing.h"

namespace Cygnus3D {
	DebugInterface::DebugInterface() {

	}

	void DebugInterface::init(GLFWwindow *window) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();


		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void DebugInterface::startUpdate() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void DebugInterface::endUpdate() {

		ImGuiIO &io = ImGui::GetIO();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow *backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}

	}

	void DebugInterface::clean() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void DebugInterface::showPerformance() {

		m_fpsTimer += Window::getDeltaTime();
		m_fpsCounter++;

		if (m_fpsTimer > 0.1) {
			m_fps = m_fpsCounter;

			m_fpsCounter = 0;
			m_fpsTimer = 0;
		}

		{
			ImGui::Begin("Performance");
		
			ImGui::Text("FPS : %d", m_fps);
		
			ImGui::End();
		}
	}

	void DebugInterface::enableInterfaceTransformation(Node *node) {

		m_focusedNode = node;

	}

	void DebugInterface::gameobjectWindow() {

		ImGui::Begin("GameObject", 0, ImGuiWindowFlags_NoResize);

		showObjectTransformations();

		ImGui::End();
	}

	void DebugInterface::showObjectTransformations() {
		if (m_focusedNode != nullptr) {

			if (ImGui::TreeNode("Transformations")) {

				ImGui::Columns(3, "Transformations", false);

				glm::vec3 currentPosition = m_focusedNode->getLocalPosition();


				ImGui::Text("X");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##posX", &currentPosition.x, 0.5f, 1.5f, "%.2f");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##scaX", &m_focusedNode->getLocalScale().x, 0.5f, 1.5f, "%.2f");
				ImGui::NextColumn();

				ImGui::Text("Y");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##posY", &currentPosition.y, 0.5f, 1.5f, "%.2f");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##scaY", &m_focusedNode->getLocalScale().y, 0.5f, 1.5f, "%.2f");
				ImGui::NextColumn();

				ImGui::Text("Z");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##posZ", &currentPosition.z, 0.5f, 1.5f, "%.2f");
				ImGui::SetNextItemWidth(80.0f);
				ImGui::InputFloat("##scaZ", &m_focusedNode->getLocalScale().z, 0.5f, 1.5f, "%.2f");

				m_focusedNode->setPosition(currentPosition);

				ImGui::TreePop();
			}
		}
	}

	void DebugInterface::postprocessingWindow() {

		bool status;

		ImGui::Begin("Post-Processing");

		status = PostProcessing::m_enabled;
		ImGui::Checkbox("Post-Processing", &PostProcessing::m_enabled);
		if (status != PostProcessing::m_enabled) {
			if (PostProcessing::m_enabled) {
				PostProcessing::enable();
				std::cout << "POST-PROCESSING ENABLED!" << std::endl;
			}
			else {
				PostProcessing::disable();
				std::cout << "POST-PROCESSING DISABLED!" << std::endl;
			}
		}

		if (PostProcessing::m_enabled) {
			status = PostProcessing::m_blackAndWhite;
			ImGui::Checkbox("Black & White filter", &PostProcessing::m_blackAndWhite);
			if (status != PostProcessing::m_blackAndWhite) {
				PostProcessing::statusBAW(PostProcessing::m_blackAndWhite);
			}
		}

		ImGui::End();
	}

	void DebugInterface::render() {

		{
			
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			
			ImGuiViewport *viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::SetNextWindowBgAlpha(0.0f);
			
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Cygnus3D", false, windowFlags);
			ImGui::PopStyleVar(3);
			
			m_coreMenu = ImGui::GetID("MyDockspace");
			ImGuiDockNodeFlags dockFlags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGui::DockSpace(m_coreMenu, ImVec2(0.0f, 0.0f), dockFlags);

			ImGui::SetNextWindowSize(ImVec2(400.0f, 500.0f));

			gameobjectWindow();
			postprocessingWindow();
			showPerformance();
			
			ImGui::End();
		}
	}

	bool DebugInterface::mouseInsideWidget() {
		return (ImGui::IsAnyWindowHovered() || ImGui::IsAnyItemHovered())  ;
	}
}