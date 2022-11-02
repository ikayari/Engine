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
	/// 当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 歩きステートの遷移
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 走りステートの遷移
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// 回避ステートの遷移
	/// </summary>
	void ProcessDadgeStateTransition();
	/// <summary>
	/// 回避ステートの遷移
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();

	void PlayAttackAnimation(int num);
	/// <summary>
	/// プレイヤー移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// プレイヤー回転処理
	/// </summary>
	void Rotation();
	// アニメーションイベント用の関数。
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Attack();

	/// <summary>
	/// フェードイン。
	/// </summary>
	void AfterImageFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// フェードアウト。
	/// </summary>
	void AfterImageFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// フェード中＞？
	/// </summary>
	/// <returns>フェード中ならtrue。</returns>
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
		enState_FadeIn,			//フェードイン中。
		enState_FadeOut,		//フェードアウト中。
		enState_Idle,			//アイドル中。
	};
	EnAfterImageState			m_state = enState_Idle;		//状態。

	CharacterController m_charaCon;

	Slow* m_slow;
	//アニメーションクリップ	
	enum EnAnimationClip {
		enAnimationClip_Idle,				//待機アニメーション。	
		enAnimationClip_Walk,				//歩きアニメーション。
		enAnimationClip_Run,				//走りアニメーション。
		enAnimationClip_Attack1,			//攻撃アニメーション1。
		enAnimationClip_Attack2,			//攻撃アニメーション2。
		enAnimationClip_Attack3,			//攻撃アニメーション3。
		enAnimationClip_Attack4,			//攻撃アニメーション4。
		enAnimationClip_Attack5,			//攻撃アニメーション5。
		enAnimationClip_Dadge,				//回避アニメーション。
		enAnimationClip_Damage,				//被ダメージアニメーション。
		//enAnimationClip_Down,				//ダウンアニメーション。
		enAnimationClip_Num,				//アニメーションの数。
	};
	enum EnPlayerState {
		enPlayerState_Idle,					//待機。
		enPlayerState_Walk,					//歩き。
		enPlayerState_Run,					//走る。
		enPlayerState_ReceiveDamage,		//ダメ―ジ受けた。
		enPlayerState_Attack,				//攻撃ステート
		enPlayerState_Dadge					//HPが0。		
	};
	EnPlayerState m_playerState = enPlayerState_Idle;			//プレイヤーステート。
	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
};

