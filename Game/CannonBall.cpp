#include "stdafx.h"
#include "CannonBall.h"
#include "Slow.h"
#include "Player.h"

CannonBall::~CannonBall()
{
	DeleteGO(m_CO);
}

bool CannonBall::Start()
{
	m_modelRender.Init("Assets/modelData/Cannon/CannonBall.tkm");
	m_position.y += 25.0f* m_scaleRatio;
	m_position += m_forward * 50.0f* m_scaleRatio;
	m_modelRender.SetTRS(m_position, m_rotation, m_scale* m_scaleRatio*0.8f);
	m_modelRender.Update();
	//コリジョンオブジェクトを作成する。
	m_CO = NewGO<CollisionObject>(0);


	m_CO->CreateSphere(m_position,               //座標。
		m_rotation,                                       //回転。
		20.0f*m_scaleRatio*0.8f                            //大きさ。
	);
	m_CO->SetName("Enemy_Attack");
	m_CO->SetIsEnableAutoDelete(false);
	m_slow = FindGO<Slow>("slow");
	return true;
}

void CannonBall::Update()
{

	m_position += m_forward * 5.0f * m_slow->GetSlowRatio();
	m_modelRender.SetPosition(m_position);
	m_CO->SetPosition(m_position);
	m_modelRender.Update();
	m_timer += g_gameTime->GetFrameDeltaTime() * m_slow->GetSlowRatio();
	if (m_timer >= 8.0f)
	{
		DeleteGO(this);
	}

}

void CannonBall::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
