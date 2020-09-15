#pragma once

#include <Bullet/LinearMath/btIDebugDraw.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <GLEW/glew.h>
#include "../shaders/shader.h"

namespace Cygnus3D {

	class DebugDrawer : public btIDebugDraw {

	public:

		DebugDrawer(){}

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {

			if (m_activated) {
				data.push_back(from.getX());
				data.push_back(from.getY());
				data.push_back(from.getZ());

				data.push_back(to.getX());
				data.push_back(to.getY());
				data.push_back(to.getZ());
			}
		
		}

		void debugDraw(glm::mat4 projectionMat, glm::mat4 viewMat, Shader *debugShader) {

			if (m_activated) {
				debugShader->enable();
				debugShader->setUniformMat4("pr_matrix", projectionMat);
				debugShader->setUniformMat4("vw_matrix", viewMat);

				glGenBuffers(1, &VBO);
				glGenVertexArrays(1, &VAO);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glBindVertexArray(VAO);
				glDrawArrays(GL_LINES, 0, data.size() / 3);
				glBindVertexArray(0);

				debugShader->disable();

				data.clear();
			}

		}

		virtual void drawSphere(const btVector3& p, btScalar radius, const btVector3& color){}

		virtual void drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha){}

		virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color){}

		virtual void reportErrorWarning(const char* warningString){}

		virtual void draw3dText(const btVector3& location, const char* textString){}

		virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; }

		virtual int  getDebugMode() const { return m_debugMode; }

		void activate(bool state) { m_activated = state; }
		inline bool getState() const { return m_activated; }


	private:

		std::vector<float> data;
		int m_debugMode;

		unsigned int VAO;
		unsigned int VBO;

		bool m_activated = false;

	};
}
