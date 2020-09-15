#include "Physics.h"
#include "../Scene/Node.h"
#include "CygnusMotionState.h"

namespace Cygnus3D {

	Physics::Physics() {

		m_collisionConfig = new btDefaultCollisionConfiguration();
		m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfig);
		m_broadphase = new btDbvtBroadphase();

		m_solver = new btSequentialImpulseConstraintSolver;

		m_dynamicWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher, m_broadphase, m_solver, m_collisionConfig);
		m_dynamicWorld->setGravity(btVector3(0, -9.8f, 0));

	}

	void Physics::addRigidBody(btRigidBody *body) {
		m_dynamicWorld->addRigidBody(body);
	}

	void Physics::deleteRigidBody(btRigidBody *body) {
		m_worldBodies.erase(body);
		m_dynamicWorld->removeRigidBody(body);
		delete body;
	}

	void Physics::updateWorld(float deltaTime) {
		if (m_dynamicWorld && m_simulationState) {
			m_dynamicWorld->stepSimulation(deltaTime);
		}
	}

	btRigidBody* Physics::createRigidBody(float mass, btTransform *originTransform, btCollisionShape *shape, Node *motherNode) {

		bool isDynamic = (mass != 0);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic) shape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState *bodyMotion = new btDefaultMotionState(*originTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, bodyMotion, shape, localInertia);
		btRigidBody *body = new btRigidBody(cInfo);
		body->setWorldTransform(*originTransform);

		addRigidBody(body);

		m_worldBodies.insert(std::pair<btRigidBody*, Node*>(body, motherNode));

		return body;

	}

	void Physics::screenToWorldRay(glm::dvec2 mousePosition, int screenWidth, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 &origin, glm::vec3 &direction) {
	
		glm::vec4 rayOrigin(
			((float)mousePosition.x / screenWidth * 2.0f - 1.0f),
			(1.0f - (float)mousePosition.y / screenHeight * 2.0f),
			-1.0f,
			1.0f
			);
	
		glm::mat4 mat = glm::inverse(projectionMatrix);
		rayOrigin = mat * rayOrigin;
		rayOrigin.z = -1.0f;
		rayOrigin.w = 0.0f;
	
		mat = glm::inverse(viewMatrix);
		rayOrigin = mat * rayOrigin;
		rayOrigin = glm::normalize(rayOrigin);
	
		origin = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
		glm::vec3 rayEnd = rayOrigin * 1000.0f;
		
	
		direction = rayEnd - origin;
	}

	Node* Physics::selectObject(glm::vec3 origin, glm::vec3 direction) {
		if (m_dynamicWorld) {

			btCollisionWorld::ClosestRayResultCallback RayCallback(
				btVector3(origin.x, origin.y, origin.z),
				btVector3(direction.x, direction.y, direction.z)
			);

			m_dynamicWorld->rayTest(
				btVector3(origin.x, origin.y, origin.z),
				btVector3(direction.x, direction.y, direction.z),
				RayCallback
			);

			if (RayCallback.hasHit()) {
				btRigidBody *body = (btRigidBody*)btRigidBody::upcast(RayCallback.m_collisionObject);
				if (body) {
					Node *selectedNode = m_worldBodies[body];
					return selectedNode;
				}
				else {
					return nullptr;
				}
			}
			else {
				return nullptr;
			}

		}
		else {
			return nullptr;
		}
	}

	

	void Physics::clean() {
		if (m_dynamicWorld) {
			delete m_dynamicWorld;
			delete m_solver;
			delete m_broadphase;
			delete m_collisionConfig;
			delete m_collisionDispatcher;
		}
	}
}