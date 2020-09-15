#pragma once

#include <Bullet/LinearMath/btMotionState.h>
#include "../Scene/Node.h"

namespace Cygnus3D {

	class CygnusMotionState : public btMotionState {

		glm::vec3 m_graphicPosition;
		glm::quat m_graphicRotation;
		btTransform m_centerOfMassOffset;
		btTransform m_startWorldTrans;

	public:

		CygnusMotionState(glm::vec3 &startPosition, glm::quat &startRotation, const btTransform& centerOfMassOffset = btTransform::getIdentity())
			:m_centerOfMassOffset(centerOfMassOffset),
			m_graphicPosition(startPosition),
			m_graphicRotation(startRotation)

		{
			m_startWorldTrans.setIdentity();
			m_startWorldTrans.setOrigin(btVector3(m_graphicPosition.x, m_graphicPosition.y, m_graphicPosition.z));
			m_startWorldTrans.setRotation(btQuaternion(m_graphicRotation.x, m_graphicRotation.y, m_graphicRotation.z, m_graphicRotation.w));
		}

		virtual void getWorldTransform(btTransform & centerOfMassWorldTrans) const
		{
			btTransform graphicTransform;
			graphicTransform.setIdentity();
			graphicTransform.setOrigin(btVector3(m_graphicPosition.x, m_graphicPosition.y, m_graphicPosition.z));
			graphicTransform.setRotation(btQuaternion(m_graphicRotation.x, m_graphicRotation.y, m_graphicRotation.z, m_graphicRotation.w));

			centerOfMassWorldTrans = graphicTransform * m_centerOfMassOffset.inverse();
		}
		virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
		{
			btTransform graphicTransform;
			graphicTransform.setIdentity();

			graphicTransform = centerOfMassWorldTrans * m_centerOfMassOffset;

			m_graphicPosition = glm::vec3(graphicTransform.getOrigin().getX(), graphicTransform.getOrigin().getY(), graphicTransform.getOrigin().getZ());
			m_graphicRotation = glm::quat(graphicTransform.getRotation().getX(), graphicTransform.getRotation().getY(), graphicTransform.getRotation().getZ(), graphicTransform.getRotation().getW());

		}




	};

}
