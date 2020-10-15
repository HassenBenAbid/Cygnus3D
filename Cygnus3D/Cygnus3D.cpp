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
	node->getMaterial()->diffuseColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	node->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	node->getMaterial()->setSpecularTexture(texture2);
	node->getMaterial()->specularPower = 20.0f;
	
	Node *node2 = new Node(new Cube(), new Material());
	node2->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
	node2->getMaterial()->setDiffuseTexture(texture);
	node2->getMaterial()->setSpecularTexture(texture2);
	node2->getMaterial()->specularPower = 20.0f;

	Node *modelNode = MeshLoader::loadMesh("resources/Models/backpack/backpack.obj", false);
	//modelNode->createBoxCollider(1.0f, currentWorld);

	Node *floor = new Node(new Cube(), new Material());
	floor->createBoxCollider(0.0f, currentWorld);
	floor->setPosition(glm::vec3(0, -2.0f, 0));
	floor->getMaterial()->diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	floor->setScale(glm::vec3(10.0f, 0.2f, 10.0f));

	//sphere
	Node *sphereNode = new Node(new Sphere(50, 50), new Material());
	sphereNode->getMaterial()->diffuseColor = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	sphereNode->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));

	Node *building = MeshLoader::loadMesh("resources/Models/doomGuard/boblampclean.md5mesh");
	building->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	building->setPosition(glm::vec3(0.0f, -1.5f, 0.0f));

	///lights
	Renderer::globalAmbient = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);

	Light *dirLight = new Light(DIRECTIONAL_LIGHT);
	dirLight->intensity = 1.0f;
	dirLight->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight->direction = glm::vec3(0.0f, -1.0f, 0.0f);

	
	Light *pointLight = new Light(POINT_LIGHT);
	pointLight->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	pointLight->position = glm::vec3(2.0f, 1.0f, 0.0f);
	pointLight->range = 5.0f;

	std::cout << pointLight->color.x << "|" << pointLight->color.y << std::endl;

	Light *greenPointLight = new Light(POINT_LIGHT);
	greenPointLight->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	greenPointLight->position = glm::vec3(1.0f, 2.0f, 0.0f);
	greenPointLight->range = 5.0f;

	Light *bluePointLight = new Light(POINT_LIGHT);
	bluePointLight->color = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	bluePointLight->position = glm::vec3(0.0f, 2.0f, 0.0f);
	bluePointLight->range = 5.0f;

	//pointLight->constant = 1.0f;
	//pointLight->linear = 0.07f;
	//pointLight->quadratic = 0.017f;

	///Post-Processing
	PostProcessing::init(WINDOW_WIDTH, WINDOW_HEIGHT);
	PostProcessing::enable();

	///renderer
	Renderer *renderer = new Renderer();
	renderer->setCamera(camera);
	//renderer->setSkybox(skyboxTexture);
	renderer->init();

	renderer->pushLight(dirLight);
	renderer->pushLight(pointLight);
	renderer->pushLight(greenPointLight);
	renderer->pushLight(bluePointLight);

	renderer->pushRender(node);
	renderer->pushRender(node2);
	renderer->pushRender(modelNode);
	//renderer->pushRender(sphereNode);
	renderer->pushRender(floor);
	//renderer->pushRender(building);

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
	//building->rotate(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	building->setScale(glm::vec3(0.05f, 0.05f, 0.05f));

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

		if (InputManager::isKeyPressed(GLFW_KEY_BACKSPACE)) {
			inputMode = !inputMode;
			InputManager::inputMode(window->getWindow(), inputMode);
			currentWorld->setSimulationState(inputMode);
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

	renderer->clean();
	currentWorld->clean();
	debug->clean();

	return 0;
}