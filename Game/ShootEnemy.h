#pragma once
#include "Enemy.h"
class Slow;
class Sound;
class Player;
class EffectManage;
class EnemyUI;
class ShootEnemy:public Enemy
{
public:
	bool Start();
	~ShootEnemy();
	void Update();
	void Render(RenderContext& rc);

	void Attack();

	void Collision();

	void Rotation();

	void ManageState();

	void PlayAnimation();

	void ProcessIdleStateTransition();
	void ProcessDamageStateTransition();
	void ProcessMoveStateTransition();
	void ProcessDeadStateTransition();
	
	void ProcessCommonStateTransition();
	/// <summary>
	/// ���W���Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ��]���Z�b�g�B
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �X�P�[�����Z�b�g
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	/// <summary>
	/// ��]���擾
	/// </summary>
	/// <returns>��]</returns>
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	/// <summary>
	/// �X�P�[�����擾
	/// </summary>
	/// <returns>�X�P�[��</returns>
	const Vector3& GetScale()const
	{
		return m_scale;
	}
	/// <summary>
	/// �O�������擾
	/// </summary>
	/// <returns>�O����</returns>
	const Vector3& GetForward()const
	{
		return m_forward;
	}
	void MoveChara(float s);
	void MoveCharaToPosition(float s);

	void EnableMovieMode()
	{
		m_MovieMode = true;
	}
	void DisableMovieMode()
	{
		m_MovieMode = false;
	}
	void SetMovePosition(Vector3& pos)
	{
		m_MovePosition = pos;
	}
private:
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Vector3 m_forward=Vector3::AxisZ;

	CharacterController m_charaCon;

	ModelRender m_modelRender;

	Player* m_player;

	Slow* m_slow;

	Sound* m_sound;

	SoundSource* m_RobotIdleSE;

	float m_timer = 0.0f;
	float m_NoDamageTimer = 0.0f;
	bool m_NoDamage = false;
	float m_HP = 2;
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Move,
		enAnimationClip_Dead,
		enAnimationClip_Damage,
		enAnimationClip_Num
	};
	enum EnShootEnemyState {
		enShootEnemyState_Idle,
		enShootEnemyState_Move,
		enShootEnemyState_Dead,
		enShootEnemyState_Damage
	};
	EnShootEnemyState m_shootEnemyState = enShootEnemyState_Idle;			//�X�e�[�g�B
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	EffectManage* m_effect;
	float m_modelClipRate = 0.0f;
	
	EnemyUI* m_UI;

	bool m_MovieMode = false;

	Vector3 m_MovePosition = Vector3::Zero;
};