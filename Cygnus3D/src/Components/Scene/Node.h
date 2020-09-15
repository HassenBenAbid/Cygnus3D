#pragma once

#include "../Transform.h"
#include "Collider.h"
#include "Animator.h"
#include <vector>

namespace Cygnus3D {

	class Mesh;
	class Material;

	class Node : public Transform {

	private:

		static unsigned int m_counterID;

		unsigned int m_id;

		Mesh *m_mesh;
		Material *m_material;

		Collider *m_collider;
		Animator *m_animator;

		Node *m_nodeParent;
		std::vector<Node*> m_children;

		bool m_focused;

		void childrenTransformChange(Node* node);

	public:

		Node();
		Node(Mesh *mesh, Material *material);
		virtual ~Node() override;

		Node(const Node *node);

		void addChild(Node *node);
		void deleteChild(unsigned int id);

		void setMesh(Mesh *mesh);
		void setMaterial(Material *material);

		void createBoxCollider(float mass, Physics *currentWorld);
		void createSphereCollider(float mass, Physics *currentWorld);

		Node* getChild(unsigned int id);
		Node* getChildByIndex(unsigned int index);
		void rotate(const glm::vec3 axis, float angle) override;

		virtual void setScale(glm::vec3 scale) override;
		virtual void setPosition(glm::vec3 position) override;

		void update(float deltaTime);

		inline Mesh* getMesh() const { return m_mesh; };
		Material* getMaterial() const;
		inline Collider* getCollider() const { return m_collider; }
		inline Node* getParent() const { return m_nodeParent; };

		size_t getChildCount();
		void setFocus(bool status);

		inline bool isFocused() const { return m_focused; }
		inline std::vector<Node*> getAllChildren() const { return m_children; };
		inline unsigned int getID() const { return m_id; }

		void setAnimator(Animator *anim);
		Animator* getAnimator() const { return m_animator; }

	};
}