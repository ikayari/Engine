#include "stdafx.h"
#include "Player.h"
#include "Slow.h"
#include "EffectManage.h"

namespace
{
	const float ZANZOU_SPACE = 75.0f;
	const float CLIP_RATE_MAX = 1.0f;
	const float CLIP_RATE_MIN = 0.5f;
}
bool Player::Start()
{
	m_clipRate = CLIP_RATE_MAX;
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/Player/Idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/Player/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/Player/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/Player/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Dadge].Load("Assets/animData/Player/dadge.tka");
	m_animationClips[enAnimationClip_Dadge].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack1].Load("Assets/animData/Player/first.tka");
	m_animationClips[enAnimationClip_Attack1].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/Player/second.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack3].Load("Assets/animData/Player/third.tka");
	m_animationClips[enAnimationClip_Attack3].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack4].Load("Assets/animData/Player/fourth.tka");
	m_animationClips[enAnimationClip_Attack4].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack5].Load("Assets/animData/Player/fifth.tka");
	m_animationClips[enAnimationClip_Attack5].SetLoopFlag(false);
	m_animationClips[enAnimationClip_DadgeAttack].Load("Assets/animData/Player/dadgeAttack.tka");
	m_animationClips[enAnimationClip_DadgeAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/Player/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
		

	m_modelRender.SetCasterShadow(true);
	m_modelRender.SetOutLineDraw(true);
	//m_modelRender.SetCasterShadow(false);


	//m_scale = Vector3::One * 1.5f;
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();



	m_zanzou1.Init("Assets/modelData/Player/player.tkm", m_animationClips, enAnimationClip_Num);
	m_zanzou1.SetScale(m_scale);
	m_zanzou1.SetRotation(m_rotation);
	m_zanzou1.SetClip(m_clipRate);
	m_zanzou1.SetCasterShadow(false);
	m_zanzou1.Update();


	m_zanzou2.Init("Assets/modelData/Player/player.tkm", m_animationClips, enAnimationClip_Num);
	m_zanzou2.SetScale(m_scale);
	m_zanzou2.SetRotation(m_rotation);
	m_zanzou2.SetClip(m_clipRate);
	m_zanzou2.SetCasterShadow(false);
	m_zanzou2.Update();


	m_zanzou3.Init("Assets/modelData/Player/player.tkm", m_animationClips, enAnimationClip_Num);
	m_zanzou3.SetScale(m_scale);
	m_zanzou3.SetRotation(m_rotation);
	m_zanzou3.SetClip(m_clipRate);
	m_zanzou3.SetCasterShadow(false);
	m_zanzou3.Update();


	m_zanzou4.Init("Assets/modelData/Player/player.tkm", m_animationClips, enAnimationClip_Num);
	m_zanzou4.SetScale(m_scale);
	m_zanzou4.SetRotation(m_rotation);
	m_zanzou4.SetClip(m_clipRate);
	m_zanzou4.SetCasterShadow(false);
	m_zanzou4.Update();

	m_modelRender.Init("Assets/modelData/Player/player.tkm", m_animationClips, enAnimationClip_Num);
	//アニメーションイベント用の関数
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_slow = FindGO<Slow>("slow");
	m_effect = FindGO<EffectManage>("effectManage");
	//m_position.y += 150.0f;
	m_charaCon.Init(20.0f, 60.0f, m_position);
	m_charaCon.SetPosition(m_position);
	return true;
}
void Player::Update()
{
	

	Move();
	Rotation();
	Collision();
	ManageState();
	Attack();
	PlayAnimation();
	m_modelRender.Update();

	m_position.x += ZANZOU_SPACE;
	m_zanzou1.SetPosition(m_position);
	m_zanzou1.SetRotation(m_rotation);
	m_zanzou1.Update();

	m_position.x -= ZANZOU_SPACE * 2;
	m_zanzou2.SetPosition(m_position);
	m_zanzou2.SetRotation(m_rotation);
	m_zanzou2.Update();

	m_position.x += ZANZOU_SPACE;
	m_position.z += ZANZOU_SPACE;
	m_zanzou3.SetPosition(m_position);
	m_zanzou3.SetRotation(m_rotation);
	m_zanzou3.Update();
	m_position.z -= ZANZOU_SPACE * 2;
	m_zanzou4.SetPosition(m_position);
	m_zanzou4.SetRotation(m_rotation);
	m_zanzou4.Update();
	m_position.z += ZANZOU_SPACE;


	switch (m_state) {
	case enState_FadeIn:
		m_clipRate -= g_gameTime->GetFrameDeltaTime() * 4.0f;
		if (m_clipRate <= CLIP_RATE_MIN) {
			m_clipRate = CLIP_RATE_MIN;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut:
		m_clipRate += g_gameTime->GetFrameDeltaTime() * 4.0f;
		if (m_clipRate >= CLIP_RATE_MAX) {
			m_clipRate = CLIP_RATE_MAX;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:

		break;
	}
	m_zanzou1.SetClip(m_clipRate);
	m_zanzou2.SetClip(m_clipRate);
	m_zanzou3.SetClip(m_clipRate);
	m_zanzou4.SetClip(m_clipRate);

}
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	if (m_clipRate < 1.0f)
	{
		m_zanzou1.Draw(rc);
		m_zanzou2.Draw(rc);
		m_zanzou3.Draw(rc);
		m_zanzou4.Draw(rc);
	}
}
void Player::ManageState()
{
	switch (m_playerState)
	{
	case enPlayerState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//歩きステートの時。
	case enPlayerState_Walk:
		//歩きステートのステート遷移処理。
		ProcessWalkStateTransition();
		//移動速度を設定。
		SetMoveSpeed(150.0f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		//走りステートのステート遷移処理。
		ProcessRunStateTransition();
		//移動速度を設定。
		SetMoveSpeed(300.0f);
		break;
	case enPlayerState_ReceiveDamage:
		//被ダメージ時のステート遷移処理
		ProcessReceiveDamageStateTransition();
		//移動速度を設定
		SetMoveSpeed(50.0f);
		break;
	case enPlayerState_Dadge:

		//回避時のステート遷移処理
		ProcessDadgeStateTransition();
		SetMoveSpeed(150.0f);
		break;
	case enPlayerState_Attack:
		ProcessAttackStateTransition();
		SetMoveSpeed(50.0f);
		break;
	case enPlayerState_Down:
		break;
	default:
		break;

	}
}
void Player::ProcessCommonStateTransition()
{
	if (m_playerState != enPlayerState_Attack)
	{
		m_attack = false;
	}
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		m_AttackNum = 0;
		m_playerState = enPlayerState_Attack;
		return;
	}

	if (g_pad[0]->IsPress(enButtonStart))
	{
		m_dadge = true;
	}
	else
	{
		m_dadge = false;
	}
	//移動しているとき
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//Bボタンを押すと走る。
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
		//押していないときは歩く。
		else
		{
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//移動していないときは待機モーション
	else
	{
		m_playerState = enPlayerState_Idle;
		return;
	}


}
void Player::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
		//待機ステートの時。
	case enPlayerState_Idle:
		//待機アニメーションを再生。
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);

		m_zanzou1.SetAnimationSpeed(1.0f);
		m_zanzou1.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou2.SetAnimationSpeed(1.0f);
		m_zanzou2.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou3.SetAnimationSpeed(1.0f);
		m_zanzou3.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou4.SetAnimationSpeed(1.0f);
		m_zanzou4.PlayAnimation(enAnimationClip_Idle, 0.2f);
		break;
		//歩きステートの時。
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.2f);
		//歩きアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.3f);

		m_zanzou1.SetAnimationSpeed(1.2f);
		m_zanzou1.PlayAnimation(enAnimationClip_Walk, 0.2f);
		m_zanzou2.SetAnimationSpeed(1.2f);
		m_zanzou2.PlayAnimation(enAnimationClip_Walk, 0.2f);
		m_zanzou3.SetAnimationSpeed(1.2f);
		m_zanzou3.PlayAnimation(enAnimationClip_Walk, 0.2f);
		m_zanzou4.SetAnimationSpeed(1.2f);
		m_zanzou4.PlayAnimation(enAnimationClip_Walk, 0.2f);
		break;
		//走りステートの時。
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);


		m_zanzou1.SetAnimationSpeed(1.2f);
		m_zanzou1.PlayAnimation(enAnimationClip_Run, 0.2f);
		m_zanzou2.SetAnimationSpeed(1.2f);
		m_zanzou2.PlayAnimation(enAnimationClip_Run, 0.2f);
		m_zanzou3.SetAnimationSpeed(1.2f);
		m_zanzou3.PlayAnimation(enAnimationClip_Run, 0.2f);
		m_zanzou4.SetAnimationSpeed(1.2f);
		m_zanzou4.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;

	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.05f);


		m_zanzou1.SetAnimationSpeed(4.0f*m_slow->GetSlowRatio() / 2.0f);
		m_zanzou1.PlayAnimation(enAnimationClip_Damage, 0.2f);
		m_zanzou2.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_zanzou2.PlayAnimation(enAnimationClip_Damage, 0.2f);
		m_zanzou3.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_zanzou3.PlayAnimation(enAnimationClip_Damage, 0.2f);
		m_zanzou4.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_zanzou4.PlayAnimation(enAnimationClip_Damage, 0.2f);
		break;
	case enPlayerState_Dadge:
		m_modelRender.SetAnimationSpeed(4.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Dadge, 0.2f);


		m_zanzou1.SetAnimationSpeed(4.0f);
		m_zanzou1.PlayAnimation(enAnimationClip_Dadge, 0.2f);
		m_zanzou2.SetAnimationSpeed(4.0f);
		m_zanzou2.PlayAnimation(enAnimationClip_Dadge, 0.2f);
		m_zanzou3.SetAnimationSpeed(4.0f);
		m_zanzou3.PlayAnimation(enAnimationClip_Dadge, 0.2f);
		m_zanzou4.SetAnimationSpeed(4.0f);
		m_zanzou4.PlayAnimation(enAnimationClip_Dadge, 0.2f);
		

		if (m_slow->IsSlow())
		{
			m_modelRender.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio()/2.0f);
			m_zanzou1.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio()/2.0f);
			m_zanzou2.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio()/2.0f);
			m_zanzou3.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio()/2.0f);
			m_zanzou4.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio()/2.0f);
			
		}

		
		break;
	case enPlayerState_Attack:
					
		PlayAttackAnimation(m_AttackNum);
		break;
	case enPlayerState_Down:
		m_modelRender.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Down);
	default:
		break;
	}

}
void Player::PlayAttackAnimation(int num)
{
	m_modelRender.SetAnimationSpeed(3.5f*m_slow->GetSlowRatio());
	m_modelRender.PlayAnimation(enAnimationClip_Attack1+num, 0.3f);
}
void Player::Move()
{
	if (m_playerState == enPlayerState_Down)
	{
		return;
	}
	//xzの移動速度を0.0fにする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	//カメラの前方向と右方向のベクトル。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;
	//正規化
	forward.Normalize();
	right.Normalize();
	//左スティックの入力量と移動速度を乗算
	right *= stickL.x * GetMoveSpeed() * m_slow->GetSlowRatio();
	forward *= stickL.y * GetMoveSpeed() * m_slow->GetSlowRatio();
	//移動速度加算
	m_moveSpeed += right + forward;
	//地面にいるときは
	if (m_charaCon.IsOnGround())
	{
		m_position.y = 0.0f;
	}
	else
	{
		m_position.y -= 5.0f;
	}
	//キャラクターコントローラーを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);
}
void Player::Rotation()
{
	//移動速度があるとき
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//回転を作成

		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//回転を適用
		m_modelRender.SetRotation(m_rotation);
	}
}
void Player::Collision()
{
	//敵の攻撃用のコリジョンの配列を取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("Enemy_Attack");
	//配列をfor分で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			if (m_playerState != enPlayerState_Dadge && m_playerState != enPlayerState_ReceiveDamage)
			{
				if (m_dadge)
				{
					m_playerState = enPlayerState_Dadge;
				}
				else
				{
					m_playerState = enPlayerState_ReceiveDamage;
					Vector3 y = Vector3::AxisY * 80.0f;

					m_effect->PlayEffect(1, m_position + y, Vector3::One * 10.0f);
					
					if (m_hp <= 0)
					{
						m_playerState = enPlayerState_Down;
						m_slow->StartSlowMotion(1.0f);
					}
					else
					{
						m_hp--;
						m_slow->StartSlowMotion(0.5f);
					}

					
				}
			}
		}

	}
}
void Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}
void Player::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}
void  Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}
void Player::ProcessReceiveDamageStateTransition()
{
	if (!m_modelRender.IsPlayingAnimation())
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
	}
}
void Player::ProcessDadgeStateTransition()
{
	
	m_modelRender.SetOutLineDraw(false);
	m_modelRender.SetClip(0.9f);
	if (!m_modelRender.IsPlayingAnimation())
	{
		//ステートを遷移する。
		ProcessCommonStateTransition();
		m_modelRender.SetOutLineDraw(true);
		m_modelRender.SetClip(0.0f);
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		m_playerState = enPlayerState_Attack;
		//ステートを遷移する。
		m_modelRender.SetOutLineDraw(true);
		m_modelRender.SetClip(0.0f);
		m_AttackNum = 5;
		AfterImageFadeOut();
		
	}
}
void Player::ProcessAttackStateTransition()
{
	if (g_pad[0]->IsPress(enButtonB))
	{
		m_playerState = enPlayerState_Run;
	}
	if (m_attackWait)
	{
		if (g_pad[0]->IsTrigger(enButtonRB1) && m_AttackNum <= 3 && m_nextAttack == false)
		{
			m_nextAttack = true;
		}
	}
	else
	{
		if (m_nextAttack)
		{
			m_AttackNum++;
			m_nextAttack = false;
		}

	}

	if (!m_modelRender.IsPlayingAnimation())
	{
		m_playerState = enPlayerState_Idle;
		m_AttackNum = 0;
		m_nextAttack = false;
	}
}
void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"slow_Start") == 0)
	{
		m_slow->StartSlowMotion(0.3f);
	}
	if (wcscmp(eventName, L"slow_Stop") == 0)
	{
		//攻撃を終わる。
		//m_slow->EndSlowMotion();
	}
	if (wcscmp(eventName, L"AfterImage_FadeOut") == 0)
	{
		//残像がフェードアウトする処理
		AfterImageFadeOut();
	}
	if (wcscmp(eventName, L"AfterImage_FadeIn") == 0)
	{
		//残像がフェードインする処理
		AfterImageFadeIn();
	}
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		//攻撃判定生成
		m_attack = true;

	}
	if (wcscmp(eventName, L"Attack_End") == 0)
	{
		//攻撃終了
		m_attack = false;
	}
	if (wcscmp(eventName, L"Attack_Wait_Start") == 0)
	{
		//連続攻撃待機開始
		m_attackWait = true;
	}
	if (wcscmp(eventName, L"Attack_Wait_End") == 0)
	{
		//連続攻撃待機終了
		m_attackWait = false;
	}
	
}

void Player::Attack()
{
	if (m_attack)
	{
		auto collision = NewGO<CollisionObject>(0);
		Vector3 Z = Vector3::AxisZ;
		m_rotation.Apply(Z);
		Vector3 pos = m_position+Z*45.0f;
		pos.y += 60.0f;
		collision->CreateBox(pos, m_rotation, { 80.0f,120.0f,60.0f });
		collision->SetName("Player_Attack");
	}
}
