#include "Collider.h"
#include "Node.h"
#include "../Rendering/Mesh.h"

namespace Cygnus3D {

	Collider::~Collider() {
		m_physicWorld->deleteRigidBody(m_body);
		delete m_shape;
	}

	Collider::Collider(const Collider *collider, Node *motherNode) {
		
		m_motherNode = motherNode;
		m_physicWorld = collider->m_physicWorld;
		m_body = new btRigidBody(*collider->m_body);

		m_shape = collider->m_shape;
		m_scale = collider->m_scale;
		m_physicTransform = collider->m_physicTransform;

		m_isActive = collider->m_isActive;
	}

	void Collider::createBoxShape(Node *currentNode, float mass) {
		if (m_shape == nullptr) {

			m_motherNode = currentNode;

			m_scale = btVector3(.5f, .5f, .5f);

			if (currentNode->getMesh() != nullptr) {

				m_scale = btVector3(currentNode->getMesh()->getMax().x 
					, currentNode->getMesh()->getMax().y 
					, currentNode->getMesh()->getMax().z);

			}

			btVector3 currentScale = btVector3(m_scale.getX() * m_motherNode->getLocalScale().x, m_scale.getY() * m_motherNode->getLocalScale().y, m_scale.getZ() * m_motherNode->getLocalScale().z);
			m_shape = new btBoxShape(currentScale);

			init(mass);
		}
	}

	void Collider::createSphereShape(Node *currentNode, float mass) {
		if (m_shape == nullptr) {
			m_motherNode = currentNode;

			m_scale = btVector3(.5f, 0, 0);

			if (currentNode->getMesh() != nullptr) {

				m_scale.setX(currentNode->getMesh()->getMax().x);
				if (m_scale.getX() < currentNode->getMesh()->getMax().y) m_scale.setX(currentNode->getMesh()->getMax().y);
				if (m_scale.getX() < currentNode->getMesh()->getMax().z) m_scale.setX(currentNode->getMesh()->getMax().z);

			}

			btVector3 currentScale = btVector3(m_scale.getX() * m_motherNode->getLocalScale().x, m_scale.getY() * m_motherNode->getLocalScale().y, m_scale.getZ() * m_motherNode->getLocalScale().z);
			m_shape = new btSphereShape(m_scale.getX());

			init(mass);
		}
	}

	void Collider::init(float mass) {

		btVector3 currentPos = btVector3(m_motherNode->getLocalPosition().x, m_motherNode->getLocalPosition().y, m_motherNode->getLocalPosition().z);
		btQuaternion currentRot = btQuaternion(m_motherNode->getRotation().x, m_motherNode->getRotation().y, m_motherNode->getRotation().z, m_motherNode->getRotation().w);
		m_physicTransform = btTransform();
		m_physicTransform.setIdentity();
		m_physicTransform.setOrigin(currentPos);
		m_physicTransform.setRotation(currentRot);

		m_body = m_physicWorld->createRigidBody(mass, &m_physicTransform, m_shape, m_motherNode);
		m_body->getMotionState()->setWorldTransform(m_physicTransform);

	}

	void Collider::setScale(float x, float y, float z) {
		m_scale = btVector3(x, y, z);

		m_shape->setLocalScaling(m_scale);
	}

	void Collider::setPosition(float x, float y, float z) {
		btVector3 position = btVector3(x, y, z);

		m_physicTransform.setOrigin(position);
		m_body->getMotionState()->setWorldTransform(m_physicTransform);
		m_body->setWorldTransform(m_physicTransform);

		btVector3 zeroVector(0, 0, 0);
		m_body->setAngularVelocity(zeroVector);
		m_body->setLinearVelocity(zeroVector);

		m_body->clearForces();
		m_body->activate(true);

	}

	void Collider::setRotation(float x, float y, float z, float w) {
		btQuaternion rotation = btQuaternion(x, y, z, w);

		m_physicTransform.setRotation(rotation);
		m_body->getMotionState()->setWorldTransform(m_physicTransform);
		m_body->setWorldTransform(m_physicTransform);
	}

	void Collider::setActive(const bool isActive) {
		m_isActive = isActive;

		m_body->setActivationState(isActive);
	}

	void Collider::update() {

		m_physicTransform = m_body->getWorldTransform();
	}

}