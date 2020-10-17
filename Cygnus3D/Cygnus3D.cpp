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

	Window *window = new Window("TEST", WINDOW_WIDTH, WINDOW_HEIGHT);
	Window::enableMSAA();

	Camera *camera = new Camera(glm::radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);
	camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

	InputManager::initCallbacks(window->getWindow(), camera);

	Texture *containerTex = new Texture("resources/texture/container.png");
	Texture *containerSpec = new Texture("resources/texture/containerSpecular.png");
	Texture *grassTexture = new Texture("resources/texture/window.png");

	std::vector<const char*> skyboxPath{
		"resources/texture/skybox/nightSky/left.png",
			"resources/texture/skybox/nightSky/right.png",
			"resources/texture/skybox/nightSky/top.png",
			"resources/texture/skybox/nightSky/bottom.png",
			"resources/texture/skybox/nightSky/front.png",
			"resources/texture/skybox/nightSky/back.png"
	};
	Cubemap *skyboxTexture = new Cubemap(skyboxPath);

	///gameobjects
	Node *coloredBox = new Node(new Cube(), new Material());
	coloredBox->getMaterial()->diffuseColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	coloredBox->setPosition(glm::vec3(1.0f, 2.0f, 0.0f));
	coloredBox->getMaterial()->setSpecularTexture(containerSpec);
	coloredBox->getMaterial()->specularPower = 20.0f;
	
	Node *woodBox = new Node(new Cube(), new Material());
	woodBox->setPosition(glm::vec3(2.0f, 0.0f, 2.0f));
	woodBox->getMaterial()->setDiffuseTexture(containerTex);
	woodBox->getMaterial()->setSpecularTexture(containerSpec);
	woodBox->getMaterial()->specularPower = 20.0f;

	Node *backpack = MeshLoader::loadMesh("resources/Models/backpack/backpack.obj", false);
	backpack->setPosition(glm::vec3(3.5f, 0.0f, -3.0f));

	Node *floor = new Node(new Cube(), new Material());
	floor->setPosition(glm::vec3(0, -2.0f, 0));
	floor->getMaterial()->diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	floor->setScale(glm::vec3(10.0f, 0.2f, 10.0f));

	Node *sphereNode = new Node(new Sphere(50, 50), new Material());
	sphereNode->createSphereCollider(1.0f, currentWorld);
	sphereNode->getMaterial()->diffuseColor = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
	sphereNode->setPosition(glm::vec3(1.5f, 4.0f, 0.0f));
	sphereNode->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

	Node *animatedModel = MeshLoader::loadMesh("resources/Models/character/character.dae");
	animatedModel->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	animatedModel->setPosition(glm::vec3(-2.0f, -2.3f, 0.0f));

	///lights
	Renderer::globalAmbient = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
	const glm::vec3 lightContainerScale = glm::vec3(0.3f, 0.3f, 0.3f);

	Light *dirLight = new Light(DIRECTIONAL_LIGHT);
	dirLight->intensity = 1.0f;
	dirLight->color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight->direction = glm::vec3(0.0f, -1.0f, 0.0f);
	
	Light *redPointLight = new Light(POINT_LIGHT);
	redPointLight->color = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	redPointLight->range = 5.0f;

	Node *redLightContainer = new Node(new Sphere(25, 25), new Material());
	redLightContainer->getMaterial()->emissiveColor = glm::vec4(1.0f, 0.0, 0.0, 1.0f);
	redLightContainer->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	redLightContainer->setScale(lightContainerScale);
	redLightContainer->makeLightContainer(redPointLight);

	Light *greenPointLight = new Light(POINT_LIGHT);
	greenPointLight->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	greenPointLight->range = 5.0f;

	Node *greenLightContainer = new Node(new Sphere(25, 25), new Material());
	greenLightContainer->getMaterial()->emissiveColor = glm::vec4(0.0f, 1.0, 0.0, 1.0f);
	greenLightContainer->setPosition(glm::vec3(2.0f, -1.0f, 2.0f));
	greenLightContainer->setScale(lightContainerScale);
	greenLightContainer->makeLightContainer(greenPointLight);

	Light *bluePointLight = new Light(POINT_LIGHT);
	bluePointLight->color = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	bluePointLight->range = 5.0f;

	Node *blueLightContainer = new Node(new Sphere(25, 25), new Material());
	blueLightContainer->getMaterial()->emissiveColor = glm::vec4(0.0f, -1.0, 1.0, 1.0f);
	blueLightContainer->setPosition(glm::vec3(-2.0f, -1.0f, -2.0f));
	blueLightContainer->setScale(lightContainerScale);
	blueLightContainer->makeLightContainer(bluePointLight);

	///Post-Processing
	PostProcessing::init(WINDOW_WIDTH, WINDOW_HEIGHT);
	PostProcessing::enable();

	///renderer
	Renderer *renderer = new Renderer();
	renderer->setCamera(camera);
	renderer->setSkybox(skyboxTexture);
	renderer->init();

	renderer->pushLight(dirLight);
	renderer->pushLight(redPointLight);
	renderer->pushLight(greenPointLight);
	renderer->pushLight(bluePointLight);

	renderer->pushRender(redLightContainer);
	renderer->pushRender(greenLightContainer);
	renderer->pushRender(blueLightContainer);

	renderer->pushRender(coloredBox);
	renderer->pushRender(woodBox);
	renderer->pushRender(backpack);
	renderer->pushRender(sphereNode);
	renderer->pushRender(floor);
	renderer->pushRender(animatedModel);

	///ImGui
	DebugInterface *debug = new DebugInterface();
	debug->init(window->getWindow());
	Node *collidedNode = nullptr;

	//debuger
	DebugDrawer *drawer = new DebugDrawer();
	currentWorld->getWorld()->setDebugDrawer(drawer);
	drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	//colliders
	floor->createBoxCollider(0.0f, currentWorld);
	coloredBox->createBoxCollider(1.0f, currentWorld);
	//modelNode->createBoxCollider(1.0f, currentWorld);

	//other transformations
	animatedModel->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

	bool inputMode = false;
	bool simulation = false;
	float timer = 0;

	while (!window->closed())
	{

		debug->startUpdate();

		debug->render();

		timer += window->getDeltaTime();
		blueLightContainer->setPosition(glm::vec3(sin(timer) * 4.0f, -1.0f, -2.0f));
		greenLightContainer->setPosition(glm::vec3(-sin(timer) * 4.0f, -1.0f, 2.0f));
		redLightContainer->setPosition(glm::vec3(0.0f, sin(timer) * 3.0f + 2.0f, 0.0f));

		backpack->rotate(glm::vec3(0.0f, 1.0f, 0.0), window->getDeltaTime() * 18.0f);

		
		if (inputMode) {
			camera->update(0.02f);
			camera->input(window->getDeltaTime() * 4.0f);
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

		}

		if (InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
			simulation = !simulation;
			currentWorld->setSimulationState(simulation);
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