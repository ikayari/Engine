#include "stdafx.h"
#include "Spider.h"
#include "Slow.h"
#include "Sound.h"
#include "Player.h"
#include "EffectManage.h"
#include "EnemyUI.h"
#include <Game.h>

bool Spider::Start()
{
	m_player = FindGO<Player>("player");
	m_sound = FindGO<Sound>("sound");

	m_effect = FindGO<EffectManage>("effectManage");
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Spider/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Spider/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/Spider/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Spider/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/Spider/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_modelRender.Init("Assets/modelData/Spider/Spider.tkm", m_animationClips, enAnimationClip_Num  );
	//アニメーションイベント用の関数
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	m_modelRender.PlayAnimation(enAnimationClip_Idle);

	m_charaCon.Init(30.0f, 30.0f, m_position);
	/*Quaternion rot = m_rotation;
	rot.AddRotationDegX(-180.0f);*/
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	m_modelRender.Update();


	m_modelRender.PlayAnimation(enAnimationClip_Idle);
	m_rotation.Apply(m_forward);
	m_forward.Normalize();
	m_slow = FindGO<Slow>("slow");
	m_UI = NewGO<EnemyUI>(0, "enemyUI");
	m_UI->SetPlayer(m_player);
	m_UI->SetMaxHP(m_HP);
	m_UI->SetNowHP(m_HP);
	return true;
}
Spider::~Spider()
{
	DeleteGO(m_UI);
}
void Spider::Rotation()
{
	if (m_spiderState == enSpider_Attack || m_spiderState == enSpider_Down||m_spiderState== enSpider_Damage)
	{
		return;
	}
	Vector3 diff;
	//エネミーからプレイヤーに向かうベクトルを計算する。
	diff = m_player->GetPosition() - m_position;
	diff.Normalize();
	m_rotation.SetRotationYFromDirectionXZ(diff);
	m_rotation.Apply(m_forward);
	m_forward.Normalize();
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
}
void Spider::Update()
{
	if (m_MovieMode)
	{
		m_UI->DisableUI();
		if (m_gravity)
		{

			if (m_charaCon.IsOnGround())
			{
				m_gravitySpeed.y = 0.0f;
			}
			else
			{
				m_gravitySpeed.y -= 15.0f;
			}
			m_position=m_charaCon.Execute(m_gravitySpeed, g_gameTime->GetFrameDeltaTime());
		}
		m_modelRender.SetTRS(m_position, m_rotation, m_scale);
		m_modelRender.Update();

		return;
	}
	m_UI->EnableUI();
	m_UI->SetEnemyPosition(m_position);
	m_UI->SetNowHP(m_HP);
	Collision();
	ManageState();
	PlayAnimation();
	Rotation();
	Vector3 zero = Vector3::Zero;
	m_position=m_charaCon.Execute(zero, 0);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

}

void Spider::Render(RenderContext& rc)
{
	if(m_modelDraw)
	{
		m_modelRender.Draw(rc);
	}
}
void Spider::Collision()
{
	if (m_spiderState == enSpider_Down)
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
		//プレイヤーの攻撃のコリジョンの配列を取得する。
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
					
					m_NoDamage = true;
					m_modelRender.PlayAnimation(enAnimationClip_Idle);
					m_spiderState = enSpider_Damage;
					Vector3 y = Vector3::AxisY * 60.0f;
					m_effect->PlayEffect(1, m_position + y, Vector3::One * 20.0f);
				}
				else
				{
					m_HP = 0;
					Vector3 y = Vector3::AxisY * 60.0f;
					m_effect->PlayEffect(1, m_position + y, Vector3::One * 20.0f);
					m_slow->StartSlowMotion(0.2f);
					m_spiderState = enSpider_Down;
					m_charaCon.RemoveRigidBoby();
					
				}
			}

		}
	}
}

void Spider::ManageState()
{
	switch (m_spiderState)
	{
	case enSpider_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
	case enSpider_Damage:
		//ダメージステートのステート遷移処理。
		ProcessDamageStateTransition();
		MoveChara(-50.0f);
		break;
	case enSpider_Attack:
		ProcessAttackStateTransition();
		break;
	case enSpider_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
	case enSpider_Walk:
		ProcessWalkStateTransition();
		MoveChara(100.0f);
		break;
	default:
		break;

	}
}

void Spider::ProcessCommonStateTransition()
{
	if (m_player->GetPlayerState() == m_player->enPlayerState_Down)
	{
		return;
	}
	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	if (diff.Length() >= 4000.0f)
	{
		m_spiderState = enSpider_Idle;
	}
	else if (diff.Length() <= 100.0f)
	{
		m_spiderState = enSpider_Attack;
	}
	else
	{		
		m_spiderState = enSpider_Walk;
	}
}

void Spider::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}
void Spider::ProcessDamageStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation())
	{
		ProcessCommonStateTransition();
	}
}
void Spider::ProcessDownStateTransition()
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
void Spider::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}
void Spider::ProcessAttackStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation())
	{
		ProcessCommonStateTransition();
		m_spiderState = enSpider_Idle;
	}
}

void Spider::PlayAnimation()
{
	switch (m_spiderState)
	{
	case enSpider_Idle:
		m_modelRender.SetAnimationSpeed(1.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
	case enSpider_Damage:
		m_modelRender.SetAnimationSpeed(0.75f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Damage);
		break;
	case enSpider_Down:
		m_modelRender.SetAnimationSpeed(2.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Down);
		break;
	case enSpider_Walk:
		m_modelRender.SetAnimationSpeed(1.0f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.02f);
		break;
	case enSpider_Attack:
		m_modelRender.SetAnimationSpeed(1.8f * m_slow->GetSlowRatio());
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.02f);
		break;
	default:
		break;

	}
}
void Spider::MoveChara(float s)
{
	Vector3 diff;
	//エネミーからプレイヤーに向かうベクトルを計算する。
	diff = m_player->GetPosition() - m_position;
	//ベクトルを正規化する。
	diff.Normalize();
	//移動速度を設定する。
	Vector3 moveSpeed = diff * s * m_slow->GetSlowRatio();
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());
	m_modelRender.SetPosition(m_position);
}
void Spider::Attack()
{
	auto collision = NewGO<CollisionObject>(0);
	Vector3 Z = Vector3::AxisZ;
	m_rotation.Apply(Z);
	Vector3 pos = m_position + Z * 50.0f;
	pos.y += 60.0f;
	collision->CreateBox(pos, m_rotation, { 40.0f,120.0f,60.0f });
	collision->SetName("Enemy_Attack");
}
void Spider::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		Attack();
	}
}