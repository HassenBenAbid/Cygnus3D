#pragma once

#include "../Rendering/Mesh.h"
#include "../../shaders/shader.h"
#include "../Camera.h"
#include "../Scene/Node.h"
#include "../Lighting/DirectionalLight.h"
#include "../Lighting/PointLight.h"
#include "../Rendering/Quad.h"

#include "../../Debug/DebugDrawer.h"

namespace Cygnus3D {

	class Material;
	class Texture;

	class Renderer{

		friend class RayCast;

	private:

		std::vector<Node*> m_renderQueue;
		std::vector<DirectionalLight*> m_dirLightQueue;
		std::vector<PointLight*> m_pointLightQueue;

		Shader *m_basicShader, *m_skyboxShader, *m_debugShader;
		Camera *m_camera;
		Node *m_skybox;

		Texture *m_lastTexture;
		Texture *m_lastSpecular;

		unsigned int m_uniformBuffer;
		bool m_culling;

		void updateShader(Node *node);
		void updateLight();
		void updateClear();

		void createUniformBuffer();
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

		void setCamera(Camera *camera);
		void setSkybox(Texture *texture);
		void pushRender(Node *node);

		void pushDirLight(DirectionalLight *light);
		void pushPointLight(PointLight *light);

		inline Camera* getCamera() const { return m_camera; };

		void init();
		void render(DebugDrawer *debugDrawer = 0);
		void update(float deltaTime);

	};
}