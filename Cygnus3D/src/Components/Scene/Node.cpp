#include "Node.h"
#include "../Rendering/Mesh.h"
#include "../Rendering/Material.h"

namespace Cygnus3D {

	unsigned int Node::m_counterID;

	Node::Node(Mesh *mesh, Material *material):
		m_mesh(mesh), m_material(material)
	{
		m_counterID++;
		m_id = m_counterID;

		m_hasChanged = true;
		m_focused = false;
	}

	Node::Node() {
		m_counterID++;
		m_id = m_counterID;

		m_focused = false;
	}

	Node::~Node() {

		delete m_mesh;
		delete m_material;
		m_children.clear();

	}

	Node::Node(const Node *node) {
		m_counterID++;
		m_id = m_counterID;

		if (node->m_mesh) m_mesh = new Mesh(node->m_mesh);
		if (node->m_material) m_material = new Material(node->m_material);

		for (int i = 0; i < node->m_children.size(); i++) {
			Node *copyCat = new Node(node->m_children[i]);
			this->addChild(copyCat);
		}

		m_focused = false;

		if (node->m_collider) m_collider = new Collider(node->m_collider, this);
	
	}

	void Node::setMesh(Mesh *mesh) {
		m_mesh = mesh;

		m_hasChanged = true;
	}

	void Node::setMaterial(Material *material) {
		m_material = material;
	}

	void Node::addChild(Node *node) {
		m_children.push_back(node);
		node->setParent(this);
	}

	void Node::deleteChild(unsigned int id) {
		
		auto nodeIndex = std::find(m_children.begin(), m_children.end(), getChild(id));

		if (nodeIndex != m_children.end()) {
			m_children.erase(nodeIndex);
		}
	}

	Node* Node::getChild(unsigned int id) {
		for (int i = 0; i < m_children.size(); i++) {
			if (m_children[i]->m_id == id) {
				return m_children[i];
			}
		}

		return nullptr;
	}

	Node* Node::getChildByIndex(unsigned int index) {

		if (index < m_children.size()) {
			return m_children[index];
		}
		else {
			return nullptr;
		}
	}

	void Node::createBoxCollider(float mass, Physics *currentWorld) {
		if (m_collider == nullptr) {
			m_collider = new Collider();
			m_collider->m_physicWorld = currentWorld;
			m_collider->createBoxShape(this, mass);
		}
	}

	void Node::createSphereCollider(float mass, Physics *currentWorld) {
		if (m_collider == nullptr) {
			m_collider = new Collider();
			m_collider->m_physicWorld = currentWorld;
			m_collider->createSphereShape(this, mass);
		}
	}

	void Node::update(float deltaTime) {
		if (m_collider != nullptr) {
			m_collider->update();

			Transform::setPosition(glm::vec3(m_collider->getPhysicTransform().getOrigin().getX(), 
				m_collider->getPhysicTransform().getOrigin().getY(), 
				m_collider->getPhysicTransform().getOrigin().getZ()));
			
			Transform::setRotation(m_collider->getPhysicTransform().getRotation().getX(),
				m_collider->getPhysicTransform().getRotation().getY(),
				m_collider->getPhysicTransform().getRotation().getZ(),
				m_collider->getPhysicTransform().getRotation().getW());
			

		}

		if (m_animator != nullptr) m_animator->update(deltaTime); 

	}

	size_t Node::getChildCount() {

		if (!m_children.empty()) return m_children.size();
		else return 0;
	}

	void Node::rotate(const glm::vec3 axis, float angle) {
		Transform::rotate(axis, angle);

		for (int i = 0; i < m_children.size(); i++) {
			childrenTransformChange(m_children[i]);
		}

	}

	void Node::childrenTransformChange(Node *node) {
		node->m_hasChanged = true;

		for (int i = 0; i < node->m_children.size(); i++) {
			childrenTransformChange(node->m_children[i]);
		}
	}

	void Node::setFocus(bool status) {
		m_focused = status;

		if (m_children.size() > 0) {
			for (int i = 0; i < m_children.size(); i++) {
				m_children[i]->m_focused = status;
			}
		}
	}
	void Node::setScale(glm::vec3 scale) {
		Transform::setScale(scale);

		if (m_collider) {
			m_collider->setScale(scale.x, scale.y, scale.z);
		}
	}

	void Node::setPosition(glm::vec3 position) {
		Transform::setPosition(position);

		if (m_collider) {
			m_collider->setPosition(position.x, position.y, position.z);
		}
	}

	Material* Node::getMaterial() const {
		if (m_material) return m_material;
		else if (m_children.size() > 0) return m_children[0]->getMaterial();
	}

	void Node::setAnimator(Animator *anim) {
		m_animator = anim;
	}

	void Node::updateTransform() {

		Transform::updateTransform();

		for (int i = 0; i < m_children.size() && m_hasChanged; i++) m_children[i]->m_hasChanged = true;

	}

}