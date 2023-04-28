#include "stdafx.h"
#include "Stage.h"

Stage::~Stage()
{
}

bool Stage::Start()
{
	m_backGround_Plane.SetHasNormalMap(false);
	m_backGround_Plane.SetCasterShadow(false);
	m_backGround_Plane.SetRecieveShadow(false);//‰e‚ðŽó‚¯‚È‚¢
	m_backGround_Plane.Init("Assets/modelData/Stage/Building/plane.tkm");
	m_backGround_Plane.SetPosition(m_position);
	m_backGround_Plane.SetScale(m_scale);
	m_backGround_Plane.SetRotation(m_rotation);
	m_backGround_Plane.Update();

	m_backGround_Building.SetCasterShadow(false);
	m_backGround_Building.SetRecieveShadow(false);
	m_backGround_Building.SetHasNormalMap(true);
	m_backGround_Building.Init("Assets/modelData/Stage/Building/tatemono.tkm");
	m_backGround_Building.SetPosition(m_position);
	m_backGround_Building.SetScale(m_scale);
	m_backGround_Building.SetRotation(m_rotation);
	m_backGround_Building.Update();


	m_backGround_Collision.Init("Assets/modelData/Stage/Building/collision.tkm");
	m_backGround_Collision.SetPosition(m_position);
	m_backGround_Collision.SetScale(m_scale);
	m_backGround_Collision.SetRotation(m_rotation);
	m_backGround_Collision.Update();
	m_PSO.CreateFromModel(m_backGround_Collision.GetModel(), m_backGround_Collision.GetModel().GetWorldMatrix());
	return true;
}

void Stage::Update()
{

}

void Stage::Render(RenderContext& rc)
{
	m_backGround_Plane.Draw(rc);
	m_backGround_Building.Draw(rc);
}
