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
		enAnimationClip_WakeIdle,			//起動状態待機アニメーション。
		enAnimationClip_DownIdle,			//待機状態待機アニメーション。	
		enAnimationClip_Wake,				//起動アニメーション。
		enAnimationClip_Down,				//待機アニメーション
		enAnimationClip_Num
	};
	enum EnShootEnemyState {
		enShootEnemyState_DownIdle,
		enShootEnemyState_WakeIdle,
		enShootEnemyState_Wake,
		enShootEnemyState_Down
	};
	EnShootEnemyState m_playerState = enShootEnemyState_DownIdle;			//プレイヤーステート。
	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	
};

