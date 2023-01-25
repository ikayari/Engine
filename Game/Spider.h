#pragma once
#include "Enemy.h"
class Slow;
class Sound;
class EffectManage;
class Player;
class Spider :  public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �����蔻�菈���B
	/// </summary>
	void Collision();
	void ManageState();
	void PlayAnimation();
	void ProcessComonStateTransition();

	void ProcessIdleStateTransition();
	void ProcessDamageStateTransition();
	void ProcessDownStateTransition();
	void ProcessWalkStateTransition();
	void ProcessAttackStateTransition();

	void Rotation();

	void Attack();

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

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
private:
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Vector3 m_forward = Vector3::AxisZ;

	CharacterController m_charaCon;

	ModelRender m_modelRender;


	Slow* m_slow;

	Sound* m_sound;

	EffectManage* m_effect;

	Player* m_player;
	float m_timer = 0.0f;
	float m_NoDamageTimer = 0.0f;
	float m_modelClipRate = 0.0f;
	bool m_NoDamage = false;
	int m_HP = 5;
	enum EnAnimationClip {
		enAnimationClip_Idle,			    //�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//���s�A�j���[�V�����B
		enAnimationClip_Damage,				//�_���[�W�A�j���[�V����
		enAnimationClip_Attack,
		enAnimationClip_Down,
		enAnimationClip_Num
	};
	enum EnSpiderState {
		enSpider_Idle,
		enSpider_Damage,
		enSpider_Walk,
		enSpider_Attack,
		enSpider_Down
	};
	EnSpiderState m_spiderState = enSpider_Idle;		//�X�p�C�_�[�X�e�[�g�B
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
};

