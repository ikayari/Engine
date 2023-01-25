#include "stdafx.h"
#include "CannonBall.h"
#include "Slow.h"
#include "Player.h"
#include "graphics/effect/EffectEmitter.h"
CannonBall::~CannonBall()
{
	DeleteGO(m_CO);
	m_effectEmitter->Stop();
	if (m_effectEmitter!=nullptr)
	{
		DeleteGO(m_effectEmitter);
	}
}

bool CannonBall::Start()
{
	m_modelRender.Init("Assets/modelData/Cannon/CannonBall.tkm");
	//m_position.y += 25.0f* m_scaleRatio;
	m_position += m_forward * 10.0f* m_scaleRatio;
	m_modelRender.SetTRS(m_position, m_rotation, m_scale* m_scaleRatio*0.2f);
	m_modelRender.Update();
	//コリジョンオブジェクトを作成する。
	m_CO = NewGO<CollisionObject>(0);


	m_CO->CreateSphere(m_position,               //座標。
		m_rotation,                                       //回転。
		20.0f*m_scaleRatio*0.2f                            //大きさ。
	);
	m_CO->SetName("Enemy_Attack");
	m_CO->SetIsEnableAutoDelete(false);
	m_slow = FindGO<Slow>("slow");
	
	//エフェクトのオブジェクトを作成する。
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(0);
	//エフェクトの大きさを指定する。
	m_effectEmitter->SetScale(Vector3::One*15.0f*m_scaleRatio*0.2f);
	//エフェクトの座標を設定する。
	m_effectEmitter->SetPosition(m_position);

	//エフェクトを再生する。
	m_effectEmitter->Play();
	return true;
}

void CannonBall::Update()
{

	m_position += m_forward * 5.0f * m_slow->GetSlowRatio();
	m_modelRender.SetPosition(m_position);
	m_CO->SetPosition(m_position);
	m_modelRender.Update();
	m_timer += g_gameTime->GetFrameDeltaTime() * m_slow->GetSlowRatio();
	m_effectEmitter->SetPosition(m_position);
	if (m_timer >= 8.0f)
	{
		DeleteGO(this);
	}

}

void CannonBall::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
