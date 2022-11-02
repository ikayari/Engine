#include "stdafx.h"
#include "Cannon.h"
#include "Slow.h"
#include "CannonBall.h"

Cannon::~Cannon()
{

}

bool Cannon::Start()
{
	m_modelRender.Init("Assets/modelData/Cannon/Cannon.tkm");
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();
	m_rotation.Apply(m_forward);
	m_forward.Normalize();
	m_slow = FindGO<Slow>("slow");
	//m_PSO.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	
	m_charaCon.Init(20.0f, 2.0f, m_position);
	return true;
}

void Cannon::Update()
{
	m_timer += g_gameTime->GetFrameDeltaTime()*m_slow->GetSlowRatio();
	if (m_timer >= 2.0f)
	{
		//弾を発射。
		auto* cannonball = NewGO<CannonBall>(0, "cannonBall");
		cannonball->SetForward(m_forward);
		cannonball->SetPosition(m_position);
		cannonball->SetScaleRatio(m_scale.x);

		m_timer = 0.0f;
	}	
	if (m_NoDamage)
	{
		m_NoDamageTimer+= g_gameTime->GetFrameDeltaTime() * m_slow->GetSlowRatio();
		if (m_NoDamageTimer >= 0.5f)
		{
			m_NoDamageTimer = 0.0f;
			m_NoDamage = false;
		}
	}
	else
	{
		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("Player_Attack");
		//配列をfor分で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				m_HP--;
				if (m_HP > 0)
				{
					m_slow->StartSlowMotion(0.4f);
					m_NoDamage = true;
					
				}
				else
				{
					m_slow->StartSlowMotion(0.6f);
					DeleteGO(this);
				}
			}

		}
	}
}

void Cannon::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
