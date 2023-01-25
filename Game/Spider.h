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
	/// 当たり判定処理。
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
	/// 座標をセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 回転をセット。
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// スケールをセット
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition()const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns>回転</returns>
	const Quaternion& GetRotation()const
	{
		return m_rotation;
	}
	/// <summary>
	/// スケールを取得
	/// </summary>
	/// <returns>スケール</returns>
	const Vector3& GetScale()const
	{
		return m_scale;
	}
	/// <summary>
	/// 前方向を取得
	/// </summary>
	/// <returns>前方向</returns>
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
		enAnimationClip_Idle,			    //待機アニメーション。	
		enAnimationClip_Walk,				//歩行アニメーション。
		enAnimationClip_Damage,				//ダメージアニメーション
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
	EnSpiderState m_spiderState = enSpider_Idle;		//スパイダーステート。
	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
};

