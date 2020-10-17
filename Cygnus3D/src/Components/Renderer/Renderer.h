#pragma once

#include "../Rendering/Mesh.h"
#include "../../shaders/shader.h"
#include "../Camera.h"
#include "../Scene/Node.h"
#include "../Lighting/Light.h"
#include "../Rendering/Quad.h"

#include "../../Debug/DebugDrawer.h"

namespace Cygnus3D {

	class Material;
	class Texture;

	#define MAX_LIGHTS 50
 
	class Renderer{

		friend class RayCast;

	private:

		int m_currentLight;
		Light *m_lights[MAX_LIGHTS];

		std::vector<Node*> m_renderQueue;
		unsigned int m_queueSize;

		Shader *m_basicShader, *m_skyboxShader, *m_debugShader;
		Camera *m_camera;
		Node *m_skybox;

		Texture *m_lastTexture;
		Texture *m_lastSpecular;
		
		bool m_culling;

		unsigned int m_lightBlock;
		unsigned int m_uniformBuffer;

		void updateShader(Node *node);
		void updateLight();

		void createGlobalUniformBuffer();
		void createLightUniformBlock();
		void renderSkybox();
		void pushRender(std::vector<Node*> childrenNodes);
		void sortByTexture();
		void sortByCulling();
		void push(Node *node);
		void push(std::vector<Node*> childrenNodes);

		void renderThisQueue(std::vector<Node*> queue);

	public:

		Renderer();
		~Renderer();

		static glm::vec4 globalAmbient;

		void setCamera(Camera *camera);
		void setSkybox(Texture *texture);
		void pushRender(Node *node);

		void pushLight(Light *light);

		inline Camera* getCamera() const { return m_camera; };

		void init();
		void render(DebugDrawer *debugDrawer = 0);
		void update(float deltaTime);

		void clean();
	};
}