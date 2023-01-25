#include "stdafx.h"
#include "ShootEnemy.h"
#include "Slow.h"
#include "CannonBall.h"
#include "Sound.h"
#include "Player.h"
bool ShootEnemy::Start()
{
	m_player = FindGO<Player>("player");
	m_sound = FindGO<Sound>("sound");
	m_modelRender.Init("Assets/modelData/ShootEnemy/ShootEnemy.tkm");

	/*Quaternion rot = m_rotation;
	rot.AddRotationDegX(-180.0f);*/
    m_modelRender.SetTRS(m_position, m_rotation, m_scale);
    m_modelRender.Update();
    m_charaCon.Init(30.0f, 30.0f, m_position);


	m_rotation.Apply(m_forward);
	m_forward.Normalize();
	m_slow = FindGO<Slow>("slow");
    return true;
}
void ShootEnemy::Update()
{
	Rotation();
	Attack();
	Collision();
}
void ShootEnemy::Attack()
{
	m_timer += g_gameTime->GetFrameDeltaTime() * m_slow->GetSlowRatio();
	if (m_timer >= 2.0f)
	{
		//弾を発射。
		auto* cannonball = NewGO<CannonBall>(0, "cannonBall");
		cannonball->SetForward(m_forward);
		cannonball->SetPosition(m_position);
		cannonball->SetScaleRatio(m_scale.x);

		m_sound->PlaySE(1, 0.05f);
		m_timer = 0.0f;
	}
}
void ShootEnemy::Collision()
{
	if (m_NoDamage)
	{
		m_NoDamageTimer += g_gameTime->GetFrameDeltaTime() * m_slow->GetSlowRatio();
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
					m_slow->StartSlowMotion(0.2f);
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

void ShootEnemy::Rotation()
{
	Vector3 direction;
	direction = m_player->GetPosition()-m_position;
	direction.Normalize();
	m_rotation.SetRotationYFromDirectionXZ(direction);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}

void ShootEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}