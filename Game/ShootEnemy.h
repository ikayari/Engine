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
	EnShootEnemyState m_shootEnemyState = enShootEnemyState_Idle;			//ステート。
	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	EffectManage* m_effect;
	float m_modelClipRate = 0.0f;
	
	EnemyUI* m_UI;

	bool m_MovieMode = false;

	Vector3 m_MovePosition = Vector3::Zero;
};