#pragma once

#include "../Physics/Physics.h"

namespace Cygnus3D {

	class Node;

	class Collider {

		friend Node;
	private:

		Physics *m_physicWorld;
		Node *m_motherNode;

		btTransform m_physicTransform;
		btCollisionShape *m_shape;
		btRigidBody *m_body;

		btVector3 m_scale;
		bool m_isActive;

		void init(float mass);
		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z, float w);

	public:

		Collider() {};
		Collider(const Collider *collider, Node *motherNode);
		~Collider();

		void createBoxShape(Node *currentNode, float mass);
		void createSphereShape(Node *currentNode, float mass);

		void setScale(float x, float y, float z);

		void setActive(const bool isActive);
		inline btTransform getPhysicTransform() const { return m_physicTransform; }

		void update();

	};
}