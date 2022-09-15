#include "k2EngineLowPreCompile.h"
#include "PhysicsStaticBox.h"


namespace nsK2EngineLow{

	PhysicsStaticBox::PhysicsStaticBox()
	{
		
	}

	void PhysicsStaticBox::Create(const nsK2EngineLow::LevelRender::BoxObjectData* objData)
	{
		m_boxCollider.Create(objData->scale*100.0f);
		RigidBodyInitData rbInfo;
		rbInfo.collider = &m_boxCollider;
		rbInfo.mass = 0.0f;
		rbInfo.restitution = objData->restitution;
		m_rigidBody.Init(rbInfo);
		m_rigidBody.SetPositionAndRotation(objData->position, objData->rotation);

		
	}


}