#include "Renderer.h"
#include "../Rendering/Material.h"
#include "../../texture/Texture.h"
#include "../Rendering/Cube.h"
#include "PostProcessing.h"
#include <glm/gtc/type_ptr.hpp>

namespace Cygnus3D {

	Renderer::Renderer() {
		
		m_basicShader = new Shader("resources/shaders/basicShader.vert", "resources/shaders/basicShader.frag");
		m_skyboxShader = new Shader("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");

		m_debugShader = new Shader("resources/shaders/debugShader.vert", "resources/shaders/debugShader.frag");

		m_culling = false;
		createUniformBuffer();
	}

	Renderer::~Renderer() {

	}

	void Renderer::init() {

		std::cout << "RENDERER INITIALIZED!" << std::endl;

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_BLEND);
		glDepthFunc(GL_LEQUAL);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera->getProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		m_basicShader->enable();
	}

	void Renderer::setCamera(Camera *camera) {
		m_camera = camera;
	}

	void Renderer::setSkybox(Texture *texture) {
		m_skybox = new Node(new Cube(), new Material());
		m_skybox->getMaterial()->m_texture = texture;
	}

	void Renderer::renderSkybox() {

		if (m_skybox) {

			if (m_culling) {
				glDisable(GL_CULL_FACE);
				m_culling = false;
			}

			m_skyboxShader->enable();

			glm::mat4 view = glm::mat4(glm::mat3(m_camera->getViewMatrix()));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
			m_skybox->getMaterial()->m_texture->bind();
			m_skyboxShader->setUniform1i("skybox", m_skybox->getMaterial()->m_texture->getTextureID());

			glBindVertexArray(m_skybox->getMesh()->vao);
			glDrawElements(m_skybox->getMesh()->m_topology, m_skybox->getMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			
			m_skybox->getMaterial()->m_texture->unbind();
			m_basicShader->enable();

		}

	}

	void Renderer::pushRender(Node *node) {

		push(node);

		sortByTexture();
		sortByCulling();
	}

	void Renderer::pushRender(std::vector<Node*> childrenNodes) {
		push(childrenNodes);

		sortByTexture();
		sortByCulling();
	}

	void Renderer::push(Node *node) {
		m_renderQueue.push_back(node);

		if (node->getAllChildren().size() > 0) {
			push(node->getAllChildren());
		}
	}

	void Renderer::push(std::vector<Node*> childrenNodes) {
		for (int i = 0; i < childrenNodes.size(); i++) {
			push(childrenNodes[i]);
		}
	}

	void Renderer::sortByTexture() {

		unsigned int lastSame = 0;

		do {
			int j = lastSame;

			while (!m_renderQueue[j]->getMaterial() || !m_renderQueue[j]->getMaterial()->m_texture)
			{
				j++;
				if (j >= m_renderQueue.size()) {
					return;
				}
			}

			if (lastSame != j) {
				std::swap(m_renderQueue[lastSame], m_renderQueue[j]);
			}

			unsigned int textureId = m_renderQueue[lastSame]->getMaterial()->m_texture->getTextureID();
			lastSame++;

			for (int i = lastSame; i < m_renderQueue.size(); i++) {
				if (m_renderQueue[i]->getMaterial() && m_renderQueue[i]->getMaterial()->m_texture && m_renderQueue[i]->getMaterial()->m_texture->getTextureID() == textureId) {
					if (i != lastSame) {
						std::swap(m_renderQueue[i], m_renderQueue[lastSame]);
						lastSame++;
					}
					else {
						lastSame++;
					}
				}
			}

		} while (lastSame < m_renderQueue.size());
	}

	void Renderer::sortByCulling() {

		for (int i = 0; i < m_renderQueue.size(); i++) {
			if (m_renderQueue[i]->getMesh() && !m_renderQueue[i]->getMesh()->m_faceCulling) {
				m_renderQueue.push_back(m_renderQueue[i]);
				m_renderQueue.erase(m_renderQueue.begin() + i);
			}
		}
	}

	void Renderer::pushDirLight(DirectionalLight *light) {
		m_dirLightQueue.push_back(light);
	}

	void Renderer::pushPointLight(PointLight *light) {
		m_pointLightQueue.push_back(light);
	}

	void Renderer::updateLight() {

		m_basicShader->setUniform3f("ambientLight", Light::ambient);
		m_basicShader->setUniform3f("viewPosition", m_camera->getLocalPosition());

		for (int i = 0; i < m_dirLightQueue.size(); i++) {
			m_basicShader->setUniform3f("dirLight.diffuse", m_dirLightQueue[i]->diffuse);
			m_basicShader->setUniform3f("dirLight.specular", m_dirLightQueue[i]->specular);
			m_basicShader->setUniform3f("dirLight.direction", m_dirLightQueue[i]->direction);
		}

		for (int i = 0; i < m_pointLightQueue.size(); i++) {

			m_basicShader->setUniform3f("pointLight.position", m_pointLightQueue[i]->position);
			m_basicShader->setUniform3f("pointLight.diffuse",  m_pointLightQueue[i]->diffuse);
			m_basicShader->setUniform3f("pointLight.specular", m_pointLightQueue[i]->specular);

			m_basicShader->setUniform1f("pointLight.constant", m_pointLightQueue[i]->constant);
			m_basicShader->setUniform1f("pointLight.linear",   m_pointLightQueue[i]->linear);
			m_basicShader->setUniform1f("pointLight.quadratic",m_pointLightQueue[i]->quadratic);
		}
	}

	void Renderer::updateShader(Node *node) {

		m_basicShader->setUniform1i("hasBones", node->getMesh()->m_hasBones);
		m_basicShader->setUniformMat4("ml_matrix", node->getTransformMatrix());

		if (node->getMaterial()) {

			if (node->getMaterial()->m_texture) {
				if (node->getMaterial()->m_texture != m_lastTexture) {
					if (m_lastTexture) m_lastTexture->unbind();
					m_lastTexture = node->getMaterial()->m_texture;
					m_basicShader->setUniform1i("material.diffuse", m_lastTexture->getTextureID());
					m_lastTexture->bind();

					if (node->getMaterial()->m_specularMap) {
						if (m_lastSpecular) m_lastSpecular->unbind();
						m_lastSpecular = node->getMaterial()->m_specularMap;
						m_basicShader->setUniform1i("material.specular", m_lastSpecular->getTextureID());
						m_lastSpecular->bind();
					}
				}
			}
			else if (m_lastTexture != nullptr) {
				m_lastTexture->unbind();
				m_lastTexture = nullptr;
				m_basicShader->setUniform1i("material.diffuse", NULL);

				if (m_lastSpecular) {
					m_lastSpecular->unbind();
					m_lastSpecular = nullptr;
					m_basicShader->setUniform1i("material.specular", NULL);
				}
			}
			m_basicShader->setUniform3f("color", node->getMaterial()->m_color);
			m_basicShader->setUniform1f("material.shininess", node->getMaterial()->m_shininess);

		}

	}

	void Renderer::updateClear() {

	}

	void Renderer::renderThisQueue(std::vector<Node*> queue) {
		for (int i = 0; i < queue.size(); i++) {
			if (queue[i]->getMesh()) {

				if (!queue[i]->getMesh()->m_faceCulling && m_culling) {
					glDisable(GL_CULL_FACE);
					m_culling = false;
				}
				else if (queue[i]->getMesh()->m_faceCulling && !m_culling) {
					glEnable(GL_CULL_FACE);
					m_culling = true;
				}

				updateShader(queue[i]);

				glBindVertexArray(queue[i]->getMesh()->vao);
				glDrawElements(queue[i]->getMesh()->m_topology, queue[i]->getMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				if (queue[i]->isFocused()) {

					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);
					m_basicShader->setUniform1i("singleColor", 1);
					queue[i]->getLocalScale() += glm::vec3(0.05f, 0.05f, 0.05f);
					
					m_basicShader->setUniformMat4("ml_matrix", queue[i]->getTransformMatrix());
					
					glBindVertexArray(queue[i]->getMesh()->vao);
					glDrawElements(queue[i]->getMesh()->m_topology, queue[i]->getMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);	
					
					queue[i]->getLocalScale() -= glm::vec3(0.05f, 0.05f, 0.05f);
					m_basicShader->setUniform1i("singleColor", 0);
					
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilMask(0xFF);


				}

			}
			else {
				if (queue[i]->getAnimator() != nullptr) {
					std::vector<glm::mat4> transformations = queue[i]->getAnimator()->getBonesTransformation();
					m_basicShader->setUniformMat4("bonesMatrix", transformations, transformations.size());
				}
			}
		}
	}

	void Renderer::render(DebugDrawer *debugDrawer) {

		PostProcessing::begin();

		m_basicShader->enable();
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera->getViewMatrix()));

		updateLight();

		renderThisQueue(m_renderQueue);
		renderSkybox();

		m_basicShader->disable();

		if (debugDrawer != nullptr) {
			debugDrawer->debugDraw(m_camera->getProjectionMatrix(), m_camera->getViewMatrix(), m_debugShader);
		}

		PostProcessing::end();

		//updateClear();

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void Renderer::createUniformBuffer() {
		glGenBuffers(1, &m_uniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 130, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformBuffer);
	}

	void Renderer::update(float deltaTime) {
		for (int i = 0; i < m_renderQueue.size(); i++) {
			m_renderQueue[i]->update(deltaTime);
		}
	}

}