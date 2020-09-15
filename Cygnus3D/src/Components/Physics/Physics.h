#pragma once 

#include <Bullet/btBulletDynamicsCommon.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <map>


namespace Cygnus3D {

	class Node;

	class Physics {
	
	private:
	
		 btDefaultCollisionConfiguration *m_collisionConfig;
		 btCollisionDispatcher *m_collisionDispatcher;
		 btBroadphaseInterface *m_broadphase;
		 btConstraintSolver *m_solver;
		 btDiscreteDynamicsWorld *m_dynamicWorld;

		 std::map<btRigidBody*, Node*> m_worldBodies;

		 bool m_simulationState = false;
	
		 void addRigidBody(btRigidBody *body);
	public:

		Physics();

		 void updateWorld(float deltaTime);
		 void setSimulationState(bool state) { m_simulationState = state; }

		 btRigidBody* createRigidBody(float mass, btTransform *originTransform, btCollisionShape *shape, Node *motherNode);
		 void deleteRigidBody(btRigidBody *body);

		 static void screenToWorldRay(glm::dvec2 mousePosition, int screenWidth, int screenHeight, glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 &origin, glm::vec3 &direction);
		 Node* selectObject(glm::vec3 origin, glm::vec3 direction);

		 inline btDiscreteDynamicsWorld* getWorld() { return m_dynamicWorld; }

		 void clean();
		 //void debugDraw();
	
	};
}