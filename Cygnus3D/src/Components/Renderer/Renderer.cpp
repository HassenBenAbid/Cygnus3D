#include "Renderer.h"
#include "../Rendering/Material.h"
#include "../../texture/Texture.h"
#include "../Rendering/Cube.h"
#include "PostProcessing.h"
#include <glm/gtc/type_ptr.hpp>

namespace Cygnus3D {

	glm::vec4 Renderer::globalAmbient;

	Renderer::Renderer() {
		
		m_basicShader = new Shader("resources/shaders/basicShader.vert", "resources/shaders/mainShader.frag");
		m_skyboxShader = new Shader("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");

		m_debugShader = new Shader("resources/shaders/debugShader.vert", "resources/shaders/debugShader.frag");

		m_culling = false;
		createGlobalUniformBuffer();

		m_currentLight = 0;
		createLightUniformBlock();

		m_queueSize = 0;
		//Shader *computeShader = new Shader("resources/shaders/lightCulling.comp");
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
		m_skybox->getMaterial()->setDiffuseTexture(texture);
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
			m_skybox->getMaterial()->getDiffuseTexture()->bind();
			m_skyboxShader->setUniform1i("skybox", m_skybox->getMaterial()->getDiffuseTexture()->getTextureID());

			glBindVertexArray(m_skybox->getMesh()->vao);
			glDrawElements(m_skybox->getMesh()->m_topology, m_skybox->getMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			
			m_skybox->getMaterial()->getDiffuseTexture()->unbind();
			m_basicShader->enable();

		}

	}

	void Renderer::pushRender(Node *node) {

		push(node);

		m_queueSize = m_renderQueue.size();

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

			while (!m_renderQueue[j]->getMaterial() || !m_renderQueue[j]->getMaterial()->getDiffuseTexture())
			{
				j++;
				if (j >= m_renderQueue.size()) {
					return;
				}
			}

			if (lastSame != j) {
				std::swap(m_renderQueue[lastSame], m_renderQueue[j]);
			}

			unsigned int textureId = m_renderQueue[lastSame]->getMaterial()->getDiffuseTexture()->getTextureID();
			lastSame++;

			for (int i = lastSame; i < m_renderQueue.size(); i++) {
				if (m_renderQueue[i]->getMaterial() && m_renderQueue[i]->getMaterial()->getDiffuseTexture() && m_renderQueue[i]->getMaterial()->getDiffuseTexture()->getTextureID() == textureId) {
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

	void Renderer::pushLight(Light *light) {
		m_lights[m_currentLight] = light;
		m_currentLight++;

	}

	void Renderer::updateLight() {

		m_basicShader->setUniform4f("globalAmbient", globalAmbient);

		glBindBuffer(GL_UNIFORM_BUFFER, m_lightBlock);
		for (int i = 0; i < m_currentLight; i++) {
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Light) * i, sizeof(Light), m_lights[i]);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//m_basicShader->setUniform1i("lightNumber", m_lights.size());
		//m_basicShader->setUniform3f("viewPosition", m_camera->getLocalPosition());
		//
		//m_basicShader->setUniform1i("allLights[0].type", m_lights[0]->type);
		//m_basicShader->setUniform1i("allLights[0].enabled", m_lights[0]->enabled);
		//
		//m_basicShader->setUniform4f("allLights[0].color", m_lights[0]->color);
		//m_basicShader->setUniform3f("allLights[0].position", m_lights[0]->position);
		//m_basicShader->setUniform3f("allLights[0].direction", m_lights[0]->direction);
		//
		//m_basicShader->setUniform1f("allLights[0].range", m_lights[0]->range);
		//m_basicShader->setUniform1f("allLights[0].intensity", m_lights[0]->intensity);
		//m_basicShader->setUniform1f("allLights[0].spotLightAngle", m_lights[0]->spotLightAngle);
	}

	void Renderer::updateShader(Node *node) {

		m_basicShader->setUniform1i("hasBones", node->getMesh()->m_hasBones);
		m_basicShader->setUniformMat4("ml_matrix", node->getTransformMatrix());

		if (node->getMaterial()) {

			m_basicShader->setUniform1f("material.opacity", node->getMaterial()->opacity);
			m_basicShader->setUniform1f("material.specularPower", node->getMaterial()->specularPower);

			m_basicShader->setUniform4f("material.diffuseColor", node->getMaterial()->diffuseColor);
			m_basicShader->setUniform4f("material.specularColor", node->getMaterial()->specularColor);
			m_basicShader->setUniform4f("material.emissiveColor", node->getMaterial()->emissiveColor);

			m_basicShader->setUniform1i("material.hasDiffuseTexture", node->getMaterial()->hasDiffuseTexture());
			m_basicShader->setUniform1i("material.hasSpecularTexture", node->getMaterial()->hasSpecularTexture());
			
			if (node->getMaterial()->hasDiffuseTexture()) {

				if (node->getMaterial()->getDiffuseTexture() != m_lastTexture) {

					if (m_lastTexture) m_lastTexture->unbind();
					m_lastTexture = node->getMaterial()->getDiffuseTexture();
					m_basicShader->setUniform1i("diffuseTexture", m_lastTexture->getTextureID());
					m_lastTexture->bind();

					if (node->getMaterial()->hasSpecularTexture()) {

						if (m_lastSpecular) m_lastSpecular->unbind();
						m_lastSpecular = node->getMaterial()->getSpecularTexture();
						m_basicShader->setUniform1i("specularTexture", m_lastSpecular->getTextureID());
						m_lastSpecular->bind();

					}

				}

			}else if (m_lastTexture != nullptr) {

				m_lastTexture->unbind();
				m_lastTexture = nullptr;
				m_basicShader->setUniform1i("diffuseTexture", NULL);

				if (m_lastSpecular) {
					m_lastSpecular->unbind();
					m_lastSpecular = nullptr;
					m_basicShader->setUniform1i("specularTexture", NULL);
				}

			}

		}

	}

	void Renderer::updateClear() {

	}

	void Renderer::renderThisQueue(std::vector<Node*> queue) {
		for (int i = 0; i < m_queueSize; i++) {
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

				//if (queue[i]->isFocused()) {
				//
				//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				//	glStencilMask(0x00);
				//	m_basicShader->setUniform1i("singleColor", 1);
				//	queue[i]->getLocalScale() += glm::vec3(0.05f, 0.05f, 0.05f);
				//	
				//	m_basicShader->setUniformMat4("ml_matrix", queue[i]->getTransformMatrix());
				//	
				//	glBindVertexArray(queue[i]->getMesh()->vao);
				//	glDrawElements(queue[i]->getMesh()->m_topology, queue[i]->getMesh()->m_indices.size(), GL_UNSIGNED_INT, 0);
				//	glBindVertexArray(0);	
				//	
				//	queue[i]->getLocalScale() -= glm::vec3(0.05f, 0.05f, 0.05f);
				//	m_basicShader->setUniform1i("singleColor", 0);
				//	
				//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
				//	glStencilMask(0xFF);
				//
				//
				//}

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

	void Renderer::createGlobalUniformBuffer() {
		glGenBuffers(1, &m_uniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 130, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformBuffer);
	}

	void Renderer::createLightUniformBlock() {

		//m_basicShader->enable();
		//
		//GLuint locationUniform = glGetUniformBlockIndex(m_basicShader->getProgram(), "lightBlock");
		//glUniformBlockBinding(m_basicShader->getProgram(), locationUniform, 10);

		glGenBuffers(1, &m_lightBlock);
		glBindBuffer(GL_UNIFORM_BUFFER, m_lightBlock);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * MAX_LIGHTS, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_lightBlock);

		//m_basicShader->disable();
	}

	void Renderer::update(float deltaTime) {
		for (int i = 0; i < m_renderQueue.size(); i++) {
			m_renderQueue[i]->update(deltaTime);
		}
	}

	void Renderer::clean() {
		for (int i = 0; i <= MAX_LIGHTS; i++) {
			delete m_lights[i];
		}

		//m_lights.clear();
	}

}