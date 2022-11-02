#pragma once
class Slow;
class Player :public IGameObject
{
public:
	bool Start();

	void Update();

	void Render(RenderContext& rc);

	void SetMoveSpeed(float speed)
	{
		m_moveSpeedRatio = speed;
	}
	const float& GetMoveSpeed()const
	{
		return m_moveSpeedRatio;
	}
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	const CharacterController& GetCharacterController()const 
	{
		return m_charaCon;
	}
private:
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J��
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J��
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J��
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J��
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J��
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J��
	/// </summary>
	void ProcessDadgeStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J��
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();

	void PlayAttackAnimation(int num);
	/// <summary>
	/// �v���C���[�ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[��]����
	/// </summary>
	void Rotation();
	// �A�j���[�V�����C�x���g�p�̊֐��B
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Attack();

	/// <summary>
	/// �t�F�[�h�C���B
	/// </summary>
	void AfterImageFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g�B
	/// </summary>
	void AfterImageFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// �t�F�[�h�����H
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true�B</returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	ModelRender m_modelRender;
	ModelRender m_zanzou1;
	ModelRender m_zanzou2;
	ModelRender m_zanzou3;
	ModelRender m_zanzou4;
	Vector3 m_position;
	Quaternion m_rotation;
	Quaternion m_rot;
	Vector3 m_scale;
	Vector3 m_moveSpeed;
	float m_moveSpeedRatio=330.0f;
	bool m_dadge=false;
	float m_dadgeGauge;

	float m_clipRate=1.0f;
	float m_timer = 0.0f;

	int m_AttackNum = 0;
	bool m_attackWait = false;
	bool m_nextAttack = false;
	//bool m_doAttack = false;
	bool m_attack = false;

	enum EnAfterImageState {
		enState_FadeIn,			//�t�F�[�h�C�����B
		enState_FadeOut,		//�t�F�[�h�A�E�g���B
		enState_Idle,			//�A�C�h�����B
	};
	EnAfterImageState			m_state = enState_Idle;		//��ԁB

	CharacterController m_charaCon;

	Slow* m_slow;
	//�A�j���[�V�����N���b�v	
	enum EnAnimationClip {
		enAnimationClip_Idle,				//�ҋ@�A�j���[�V�����B	
		enAnimationClip_Walk,				//�����A�j���[�V�����B
		enAnimationClip_Run,				//����A�j���[�V�����B
		enAnimationClip_Attack1,			//�U���A�j���[�V����1�B
		enAnimationClip_Attack2,			//�U���A�j���[�V����2�B
		enAnimationClip_Attack3,			//�U���A�j���[�V����3�B
		enAnimationClip_Attack4,			//�U���A�j���[�V����4�B
		enAnimationClip_Attack5,			//�U���A�j���[�V����5�B
		enAnimationClip_Dadge,				//����A�j���[�V�����B
		enAnimationClip_Damage,				//��_���[�W�A�j���[�V�����B
		//enAnimationClip_Down,				//�_�E���A�j���[�V�����B
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	enum EnPlayerState {
		enPlayerState_Idle,					//�ҋ@�B
		enPlayerState_Walk,					//�����B
		enPlayerState_Run,					//����B
		enPlayerState_ReceiveDamage,		//�_���\�W�󂯂��B
		enPlayerState_Attack,				//�U���X�e�[�g
		enPlayerState_Dadge					//HP��0�B		
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//�v���C���[�X�e�[�g�B
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
};

