#include "stdafx.h"
#include "ShootEnemy.h"
#include "Slow.h"
#include "CannonBall.h"
#include "Sound.h"
#include "Player.h"
#include "EffectManage.h"
#include "EnemyUI.h"
#include "Game.h"

bool ShootEnemy::Start()
{
	m_player = FindGO<Player>("player");
	m_sound = FindGO<Sound>("sound");
	m_RobotIdleSE = m_sound->PlaySE(2,0.1f,true);
	
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/ShootEnemy/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Move].Load("Assets/animData/ShootEnemy/Idle.tka");
	m_animationClips[enAnimationClip_Move].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/ShootEnemy/Damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Dead].Load("Assets/animData/ShootEnemy/Down.tka");
	m_animationClips[enAnimationClip_Dead].SetLoopFlag(false);
	m_modelRender.Init("Assets/modelData/ShootEnemy/ShootEnemy.tkm",m_animationClips,enAnimationClip_Num);


	/*Quaternion rot = m_rotation;
	rot.AddRotationDegX(-180.0f);*/
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
    m_modelRender.Update();
    m_charaCon.Init(60.0f, 60.0f, m_position);

	m_rotation.Apply(m_forward);
	m_forward.Normalize();
	m_slow = FindGO<Slow>("slow");
	m_UI = NewGO<EnemyUI>(0, "enemyUI");
	m_UI->SetPlayer(m_player);
	m_UI->SetMaxHP(m_HP);
	m_UI->SetNowHP(m_HP);
    return true;
}
ShootEnemy::~ShootEnemy()
{
	DeleteGO(m_UI);
	DeleteGO(m_RobotIdleSE);
}
void ShootEnemy::Update()
{
	if (m_MovieMode)
	{
		m_UI->DisableUI();
		m_modelRender.SetTRS(m_position, m_rotation, m_scale);
		m_modelRender.Update();
		MoveChara(0.0f);
		return;
	}
	if (m_shootEnemyState == enShootEnemyState_Dead)
	{
		m_RobotIdleSE->Pause();
	}
	m_UI->EnableUI();
	m_UI->SetEnemyPosition(m_position);
	m_UI->SetNowHP(m_HP);
	Rotation();
	ManageState();
	PlayAnimation();
	Attack();
	Collision();
}
void ShootEnemy::Attack()
{
	if (m_shootEnemyState == enShootEnemyState_Dead)
	{
		return;
	}
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
	if (m_shootEnemyState == enShootEnemyState_Dead)
	{
		return;
	}
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
					m_timer = 0.0f;
					Vector3 y = Vector3::AxisY * 60.0f;
					m_effect->PlayEffect(1, m_position + y, Vector3::One * 20.0f);
					m_shootEnemyState = enShootEnemyState_Damage;
					m_modelRender.SetAnimationSpeed(1.0f * m_slow->GetSlowRatio());
					m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
				}
				else
				{
					m_HP = 0;
					m_slow->StartSlowMotion(0.6f);
					Vector3 y = Vector3::AxisY * 60.0f;
					m_effect->PlayEffect(1, m_position + y, Vector3::One * 20.0f);
					m_shootEnemyState = enShootEnemyState_Dead;
					m_charaCon.RemoveRigidBoby();
				}
			}

		}
	}
}
void ShootEnemy::ManageState()
{
	switch (m_shootEnemyState)
	{
	case enShootEnemyState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
	case enShootEnemyState_Move:
		ProcessMoveStateTransition();
		break;
	case enShootEnemyState_Damage:
		ProcessDamageStateTransition();
		break;
	case enShootEnemyState_Dead:
		ProcessDeadStateTransition();
		break;
	default:
		break;

	}
}
void ShootEnemy::PlayAnimation()
{
	switch (m_shootEnemyState)
	{
	case enShootEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(1.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
	case enShootEnemyState_Move:
		m_modelRender.SetAnimationSpeed(1.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
	case enShootEnemyState_Damage:
		m_modelRender.SetAnimationSpeed(3.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.2f);
		break;
	case enShootEnemyState_Dead:
		m_modelRender.SetAnimationSpeed(1.5f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Dead, 0.2f);
		break;
	default:
		break;

	}
	m_modelRender.Update();
}
void ShootEnemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}
void ShootEnemy::ProcessDamageStateTransition()
{
	MoveChara(-20.0f);
	if (!m_modelRender.IsPlayingAnimation())
	{
		ProcessCommonStateTransition();
	}
}
void ShootEnemy::ProcessMoveStateTransition()
{
	ProcessCommonStateTransition();
}
void ShootEnemy::ProcessDeadStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation())
	{
		m_modelClipRate += g_gameTime->GetFrameDeltaTime() * 4.0f;
		m_modelRender.SetClip(m_modelClipRate);
		if (m_modelClipRate > 1.0f)
		{
			FindGO<Game>("game")->subEnemy();
			DeleteGO(this);
		}
	}
}
void ShootEnemy::ProcessCommonStateTransition()
{
	Vector3 length;
	length = m_player->GetPosition() - m_position;
	if (length.Length() <= 500.0f)
	{
		MoveChara(-10.0f);
		m_shootEnemyState = enShootEnemyState_Move;
	}
}
void ShootEnemy::Rotation()
{
	if (m_shootEnemyState == enShootEnemyState_Dead)
	{
		return;
	}
	Vector3 direction;
	direction = m_player->GetPosition()-m_position;
	direction.Normalize();
	m_rotation.SetRotationYFromDirectionXZ(direction);
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}

void ShootEnemy::MoveChara(float s)
{
	Vector3 diff;
	Vector3 playerXZ;
	playerXZ = m_player->GetPosition();
	//エネミーからプレイヤーに向かうベクトルを計算する。
	diff = playerXZ.GetXZ() - m_position.GetXZ();
	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	Vector3 moveSpeed = diff * s * m_slow->GetSlowRatio();
	moveSpeed.y = 0.0f;
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}
void ShootEnemy::MoveCharaToPosition(float s)
{
	Vector3 diff;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	diff = m_MovePosition - m_position;
	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	Vector3 moveSpeed = diff * s;
	m_position += moveSpeed;
	m_charaCon.SetPosition(m_position);
	Vector3 modelPosition = m_position;
	modelPosition.y += 80.0f;
	m_modelRender.SetPosition(modelPosition);
}

void ShootEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}