#pragma once
#include "Enemy.h"
class Slow;
class Sound;
class ShootEnemy:public Enemy
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
private:
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Vector3 m_forward=Vector3::AxisZ;

	CharacterController m_charaCon;

	ModelRender m_modelRender;


	Slow* m_slow;

	Sound* m_sound;

	float m_timer = 0.0f;
	float m_NoDamageTimer = 0.0f;
	bool m_NoDamage = false;
	int m_HP = 5;
	enum EnAnimationClip {
		enAnimationClip_WakeIdle,			//�N����ԑҋ@�A�j���[�V�����B
		enAnimationClip_DownIdle,			//�ҋ@��ԑҋ@�A�j���[�V�����B	
		enAnimationClip_Wake,				//�N���A�j���[�V�����B
		enAnimationClip_Down,				//�ҋ@�A�j���[�V����
		enAnimationClip_Num
	};
	enum EnShootEnemyState {
		enShootEnemyState_DownIdle,
		enShootEnemyState_WakeIdle,
		enShootEnemyState_Wake,
		enShootEnemyState_Down
	};
	EnShootEnemyState m_playerState = enShootEnemyState_DownIdle;			//�v���C���[�X�e�[�g�B
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	
};

