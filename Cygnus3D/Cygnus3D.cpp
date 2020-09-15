#include <iostream>

#include "src/Base/Window.h"
#include "src/Base/InputManager.h"
#include "src/Components/Renderer/Renderer.h"
#include "src/Components/Rendering/Cube.h"
#include "src/Components/Rendering/Material.h"
#include "src/Debug/DebugInterface.h"
#include "src/mesh/MeshLoader.h"
#include "src/Components/Rendering/Quad.h"
#include "src/Components/Rendering/Sphere.h"
#include "src/texture/Cubemap.h"
#include "src/Components/Renderer/PostProcessing.h"
#include "src/Components/Physics/Physics.h"
#include "src/Debug/DebugDrawer.h"

using namespace Cygnus3D;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

int main() {

	Physics *currentWorld = new Physics();

	bool inputMode = false;

	Window *window = new Window("TEST", WINDOW_WIDTH, WINDOW_HEIGHT);
	Window::enableMSAA();

	Camera *camera = new Camera(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

	camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	InputManager::initCallbacks(window->getWindow(), camera);

	Texture *texture = new Texture("resources/texture/container.png");
	Texture *texture2 = new Texture("resources/texture/containerSpecular.png");
	Texture *grassTexture = new Texture("resources/texture/window.png");

	std::vector<const char*> skyboxPath{
		"resources/texture/skybox/right.jpg",
			"resources/texture/skybox/left.jpg",
			"resources/texture/skybox/top.jpg",
			"resources/texture/skybox/bottom.jpg",
			"resources/texture/skybox/front.jpg",
			"resources/texture/skybox/back.jpg"
	};
	Cubemap *skyboxTexture = new Cubemap(skyboxPath);

	///gameobjects
	Node *node = new Node(new Cube(), new Material());
	node->getMaterial()->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	node->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	node->getMaterial()->setSpecularMap(texture2);
	node->getMaterial()->setShininess(20.0f);
	
	Node *node2 = new Node(new Cube(), new Material());
	node2->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
	node2->getMaterial()->setTexture(texture);
	node2->getMaterial()->setSpecularMap(texture2);
	node2->getMaterial()->setShininess(20.0f);

	Node *modelNode = MeshLoader::loadMesh("resources/Models/backpack/backpack.obj", false);
	//modelNode->createBoxCollider(1.0f, currentWorld);

	Node *floor = new Node(new Cube(), new Material());
	floor->createBoxCollider(0.0f, currentWorld);
	floor->setPosition(glm::vec3(0, -2.0f, 0));
	floor->getMaterial()->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	floor->setScale(glm::vec3(10.0f, 0.2f, 10.0f));

	//sphere
	Node *sphereNode = new Node(new Sphere(50, 50), new Material());
	sphereNode->getMaterial()->setColor(glm::vec3(1.0f, 0.5f, 0.0f));
	sphereNode->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));

	Node *building = MeshLoader::loadMesh("resources/Models/doomGuard/boblampclean.md5mesh");
	building->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	building->setPosition(glm::vec3(0.0f, -1.5f, 0.0f));

	///lights
	Light::ambient = glm::vec3(0.3f, 0.3f, 0.3f);

	DirectionalLight *dirLight = new DirectionalLight();
	dirLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	dirLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLight->direction = glm::vec3(-0.2f, -1.0f, -0.3f);

	PointLight *pointLight = new PointLight();
	pointLight->position = glm::vec3(0.0f, 0.0f, -2.0f);
	pointLight->diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	pointLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->constant = 1.0f;
	pointLight->linear = 0.07f;
	pointLight->quadratic = 0.017f;

	///Post-Processing
	PostProcessing::init(WINDOW_WIDTH, WINDOW_HEIGHT);
	PostProcessing::enable();

	///renderer
	Renderer *renderer = new Renderer();
	renderer->setCamera(camera);
	renderer->setSkybox(skyboxTexture);
	renderer->init();
	renderer->pushDirLight(dirLight);
	renderer->pushPointLight(pointLight);
	//renderer->pushRender(node);
	//renderer->pushRender(node2);
	//renderer->pushRender(modelNode);
	//renderer->pushRender(sphereNode);
	//renderer->pushRender(floor);
	renderer->pushRender(building);

	//unsigned int count;
	//glm::mat4 *transforms = building->getAnimator()->getBonesTransformation(count);
	//for (int i = 0; i < count; i++) {
	//	std::cout << transforms[i][0][0] << " / ";
	//	std::cout << transforms[i][0][1] << " / ";
	//	std::cout << transforms[i][0][2] << " / ";
	//	std::cout << transforms[i][0][3] << std::endl;
	//
	//	std::cout << transforms[i][1][0] << " / ";
	//	std::cout << transforms[i][1][1] << " / ";
	//	std::cout << transforms[i][1][2] << " / ";
	//	std::cout << transforms[i][1][3] << std::endl;
	//
	//	std::cout << transforms[i][2][0] << " / ";
	//	std::cout << transforms[i][2][1] << " / ";
	//	std::cout << transforms[i][2][2] << " / ";
	//	std::cout << transforms[i][2][3] << std::endl;
	//
	//	std::cout << transforms[i][3][0] << " / ";
	//	std::cout << transforms[i][3][1] << " / ";
	//	std::cout << transforms[i][3][2] << " / ";
	//	std::cout << transforms[i][3][3] << std::endl;
	//	std::cout << "||" << std::endl;
	//}

	///ImGui
	DebugInterface *debug = new DebugInterface();
	debug->init(window->getWindow());
	Node *collidedNode = nullptr;

	//debuger
	DebugDrawer *drawer = new DebugDrawer();
	currentWorld->getWorld()->setDebugDrawer(drawer);
	drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	Shader *debugShader = new Shader("resources/shaders/debugShader.vert", "resources/shaders/debugShader.frag");

	//modelNode->getCollider()->setScale(2.0f, 3.0f, 1.0f);
	modelNode->setPosition(glm::vec3(3.0f, 0.0f, -5.0f));
	modelNode->rotate(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	node->createBoxCollider(1.0f, currentWorld);
	sphereNode->createSphereCollider(1.0f, currentWorld);
	//building->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	building->setScale(glm::vec3(0.1f, 0.1f, 0.1f));

	while (!window->closed())
	{

		debug->startUpdate();

		debug->render();
		
		if (inputMode) {
			camera->update(0.02f);
			camera->input(0.002f);
		}
		else {
			if (InputManager::isButtonReleased(GLFW_MOUSE_BUTTON_1)) {
			
				if ((collidedNode != nullptr && !debug->mouseInsideWidget()) || (collidedNode == nullptr)) {

					if (collidedNode != nullptr) {
						collidedNode->setFocus(false);
					}
			
					glm::vec3 origin;
					glm::vec3 direction;
					Physics::screenToWorldRay(InputManager::getCursorPosition(), WINDOW_WIDTH, WINDOW_HEIGHT, camera->getProjectionMatrix(), camera->getViewMatrix(), origin, direction);
			
					collidedNode = currentWorld->selectObject(origin, direction);
			
					if (collidedNode != nullptr) {
						collidedNode->setFocus(true);
					}
			
				}
			}
		}

		debug->enableInterfaceTransformation(collidedNode);


		//node2->rotate(glm::vec3(1.0f, .0f, 0.0f), 0.0005f);
		//building->rotate(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f);
		//modelNode->getLocalPosition().x += 0.001f;

		if (InputManager::isKeyPressed(GLFW_KEY_BACKSPACE)) {
			inputMode = !inputMode;
			InputManager::inputMode(window->getWindow(), inputMode);
			currentWorld->setSimulationState(inputMode);

			//building->rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
		}

		if (InputManager::isKeyPressed(GLFW_KEY_F1)) drawer->activate(!drawer->getState());


		currentWorld->updateWorld(window->getDeltaTime());
		renderer->update(window->getDeltaTime());

		currentWorld->getWorld()->debugDrawWorld();

		renderer->render(drawer);

		InputManager::clear();
		InputManager::update();

		debug->endUpdate();

		window->update();
		window->clear();
	}

	currentWorld->clean();
	debug->clean();

	return 0;
}