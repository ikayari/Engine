#include "stdafx.h"
#include "Player.h"
#include "Slow.h"
#include "EffectManage.h"
#include "PlayerUI.h"

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
	m_animationClips[enAnimationClip_Step].Load("Assets/animData/Player/step.tka");
	m_animationClips[enAnimationClip_Step].SetLoopFlag(false);
		

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
	//�A�j���[�V�����C�x���g�p�̊֐�
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_rotation.Apply(m_forward);
	m_forward.Normalize();

	m_slow = FindGO<Slow>("slow");
	m_effect = FindGO<EffectManage>("effectManage");
	//m_position.y += 150.0f;
	m_charaCon.Init(20.0f, 60.0f, m_position);
	m_charaCon.SetPosition(m_position);
	m_playerHP = NewGO<PlayerUI>(0, "playerUI");
	m_playerHP->SetMaxHP(m_hp);
	m_playerHP->SetNowHP(m_hp);
	return true;
}
void Player::Update()
{
	
	if (m_MovieMode)
	{
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 1.0f);
		m_modelRender.SetTRS(m_position, m_rotation, m_scale);
		m_modelRender.Update();
		return;
	}
	
	m_playerHP->SetNowHP(m_hp);
	Rotation();
	Move();
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
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		//�����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessWalkStateTransition();
		//�ړ����x��ݒ�B
		SetMoveSpeed(150.0f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		//����X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessRunStateTransition();
		//�ړ����x��ݒ�B
		SetMoveSpeed(300.0f);
		break;
	case enPlayerState_Step:
		ProcessStepStateTransition();
		SetMoveSpeed(400.0f);
		break;
	case enPlayerState_ReceiveDamage:
		//��_���[�W���̃X�e�[�g�J�ڏ���
		ProcessReceiveDamageStateTransition();
		//�ړ����x��ݒ�
		SetMoveSpeed(50.0f);
		break;
	case enPlayerState_Dadge:

		//������̃X�e�[�g�J�ڏ���
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
	m_dadge = false;
	if (m_playerState != enPlayerState_Attack)
	{
		m_attack = false;
	}
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		m_AttackNum =0;
		m_playerState = enPlayerState_Attack;
		return;
	}


	if (g_pad[0]->IsPress(enButtonB))
	{
		m_playerState = enPlayerState_Step;
		return;
	}
	//�ړ����Ă���Ƃ�
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//B�{�^���������Ƒ���B
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_playerState = enPlayerState_Step;
			return;
		}
		//�����Ă��Ȃ��Ƃ��͕����B
		else
		{
			m_playerState = enPlayerState_Walk;
			return;
		}

	}
	//�ړ����Ă��Ȃ��Ƃ��͑ҋ@���[�V����
	else
	{
		m_playerState = enPlayerState_Idle;
		return;
	}


}
void Player::PlayAnimation()
{
	if (m_playerState != enPlayerState_Dadge)
	{
		m_zanzou1.SetAnimationSpeed(1.2f);
		m_zanzou1.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou2.SetAnimationSpeed(1.2f);
		m_zanzou2.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou3.SetAnimationSpeed(1.2f);
		m_zanzou3.PlayAnimation(enAnimationClip_Idle, 0.2f);
		m_zanzou4.SetAnimationSpeed(1.2f);
		m_zanzou4.PlayAnimation(enAnimationClip_Idle, 0.2f);
	}
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enPlayerState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);		
		break;
		//�����X�e�[�g�̎��B
	case enPlayerState_Walk:
		m_modelRender.SetAnimationSpeed(1.2f);
		//�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.3f);
		break;
		//����X�e�[�g�̎��B
	case enPlayerState_Run:
		m_modelRender.SetAnimationSpeed(1.2f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.3f);
		break;

	case enPlayerState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.05f);
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
	case enPlayerState_Step:
		m_modelRender.SetAnimationSpeed(3.0f);
		//�����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Step,0.2f);
		break;
	case enPlayerState_Attack:					
		PlayAttackAnimation(m_AttackNum);
		break;
	case enPlayerState_Down:
		m_modelRender.SetAnimationSpeed(4.0f * m_slow->GetSlowRatio() / 2.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Down);
		break;
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
	//xz�̈ړ����x��0.0f�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	if (m_playerState == enPlayerState_Step)
	{
		Vector3 move;
		//xz�̈ړ����x��0.0f�ɂ���B
		move.x = 0.0f;
		move.z = 0.0f;
		//���X�e�B�b�N�̓��͗ʂ��擾�B
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();
		//�J�����̑O�����ƉE�����̃x�N�g���B
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//y�����ɂ͈ړ������Ȃ��B
		forward.y = 0.0f;
		right.y = 0.0f;
		//���K��
		forward.Normalize();
		right.Normalize();
		//���X�e�B�b�N�̓��͗ʂƈړ����x����Z
		right *= stickL.x;
		forward *= stickL.y;
		//�ړ����x���Z
		move += right + forward;
		move.Normalize();
		Vector3 speed = m_forward;
		Vector3 Movespeed;
		Movespeed.Lerp(0.1f, speed, move);
		Movespeed.Normalize();
		Movespeed *= GetMoveSpeed();
		m_rotation.SetRotationYFromDirectionXZ(Movespeed);
		
		m_position=m_charaCon.Execute(Movespeed, 1.0f / 60.0f);
		m_modelRender.SetPosition(m_position);
		m_modelRender.Update();
		return;
	}
	if (m_playerState == enPlayerState_Down)
	{
		return;
	}

	//���X�e�B�b�N�̓��͗ʂ��擾�B
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();
	//�J�����̑O�����ƉE�����̃x�N�g���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	//���K��
	forward.Normalize();
	right.Normalize();
	//���X�e�B�b�N�̓��͗ʂƈړ����x����Z
	right *= stickL.x * GetMoveSpeed() * m_slow->GetSlowRatio();
	forward *= stickL.y * GetMoveSpeed() * m_slow->GetSlowRatio();
	//�ړ����x���Z
	m_moveSpeed += right + forward;
	//�n�ʂɂ���Ƃ���
	if (m_charaCon.IsOnGround())
	{
		m_position.y = 0.0f;
	}
	else
	{
		m_position.y -= 5.0f;
	}
	//�L�����N�^�[�R���g���[���[���g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, 1.0f / 60.0f);
	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);
}
void Player::Rotation()
{

	//�ړ����x������Ƃ�
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//��]���쐬
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
		//��]��K�p
		m_modelRender.SetRotation(m_rotation);
	}
	if (m_playerState == enPlayerState_Step)
	{	

		//��]��K�p
		m_modelRender.SetRotation(m_rotation);
	}
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}
void Player::Collision()
{
	if (m_playerState == enPlayerState_Down)
	{
		return;
	}
	//�G�̍U���p�̃R���W�����̔z����擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("Enemy_Attack");
	//�z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
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
					m_hp--;
					m_effect->PlayEffect(1, m_position + y, Vector3::One * 10.0f);					
					if (m_hp <= 0)
					{
						m_playerState = enPlayerState_Down;
						m_slow->StartSlowMotion(1.0f);
					}
					else
					{						
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
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
	}
}
void Player::ProcessDadgeStateTransition()
{
	
	m_modelRender.SetOutLineDraw(false);
	m_modelRender.SetClip(0.9f);
	if (!m_modelRender.IsPlayingAnimation())
	{
		//�X�e�[�g��J�ڂ���B
		ProcessCommonStateTransition();
		m_modelRender.SetOutLineDraw(true);
		m_modelRender.SetClip(0.0f);
	}
	else if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		m_playerState = enPlayerState_Attack;
		//�X�e�[�g��J�ڂ���B
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
void Player::ProcessStepStateTransition()
{
	m_dadge = true;
	if (!m_modelRender.IsPlayingAnimation())
	{
		ProcessDadgeStateTransition();
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
		//�U�����I���B
		//m_slow->EndSlowMotion();
	}
	if (wcscmp(eventName, L"AfterImage_FadeOut") == 0)
	{
		//�c�����t�F�[�h�A�E�g���鏈��
		AfterImageFadeOut();
	}
	if (wcscmp(eventName, L"AfterImage_FadeIn") == 0)
	{
		//�c�����t�F�[�h�C�����鏈��
		AfterImageFadeIn();
	}
	if (wcscmp(eventName, L"Attack_Start") == 0)
	{
		//�U�����萶��
		m_attack = true;

	}
	if (wcscmp(eventName, L"Attack_End") == 0)
	{
		//�U���I��
		m_attack = false;
	}
	if (wcscmp(eventName, L"Attack_Wait_Start") == 0)
	{
		//�A���U���ҋ@�J�n
		m_attackWait = true;
	}
	if (wcscmp(eventName, L"Attack_Wait_End") == 0)
	{
		//�A���U���ҋ@�I��
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
		collision->CreateBox(pos, m_rotation, {100.0f,150.0f,100.0f});
		collision->SetName("Player_Attack");
	}
}
