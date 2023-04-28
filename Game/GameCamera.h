#pragma once
#include <SpringCamera.h>

class Game;
class Player;
//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	GameCamera() {};
	~GameCamera();
	bool Start();
	void Update();

	void ProcessNormal();
	void ProcessTarget();
	void CameraMove();
	void TargetMove();
	/// <summary>
	/// カメラの移動する位置
	/// </summary>
	/// <param name="pos">座標</param>
	void SetCameraPosition(Vector3& pos)
	{
		m_targetPosition = pos;
	}
	/// <summary>
	/// 目指す注視点の位置
	/// </summary>
	/// <param name="pos">座標</param>
	void SetTarget(Vector3& pos)
	{
		m_Target = pos;
	}
	/// <summary>
	/// カメラが加減速して動く
	/// </summary>
	/// <param name="b"></param>
	void SetCameraMove(bool b)
	{
		m_cameraMove = b;
	}
	/// <summary>
	/// 注視点が加減速して動く。
	/// </summary>
	/// <param name="b"></param>
	void SetTargetMove(bool b)
	{
		m_targetMove = b;
	}
	/// <summary>
	/// 注視点をプレイヤーにする？
	/// </summary>
	/// <param name="b"></param>
	void SetTargetPlayer(bool b)
	{
		m_TargetPlayer = b;
	}
	/// <summary>
	/// 通常に戻す
	/// </summary>
	void ReverseNormal()
	{
		m_reverse = true;
		m_TargetPlayer = true;
		m_cameraMove = true;
		m_targetPosition = m_position;
	}

	bool isMovingCamera()
	{
		return m_isCameraMoving;
	}
	bool isMovingTarget()
	{
		return m_isTargetMoving;
	}
	void ResetCameraMoveVelocity()
	{
		m_cameraVelocity = Vector3::Zero;
	}
	void ResetTargetMoveVelocity()
	{
		m_targetVelocity = Vector3::Zero;
	}
	void SetMovieCamera(bool b)
	{
		m_movieCamera = b;
	}
	void SetCameraRotateDirection(Vector3 direction,Vector3 targetPos)
	{
		m_toCameraPos.Set(0.0f, 150.0f, -250.0f);
		m_quaternion.SetRotationYFromDirectionXZ(direction);
		m_quaternion.Apply(m_toCameraPos);
		//注視点を計算する。
		Vector3 target = targetPos;
		//プレイヤの足元からちょっと上を注視点とする。
		target.y += 80.0f;
		target += g_camera3D->GetForward() * 20.0f;
		//視点を計算する。
		Vector3 pos = target + m_toCameraPos;
		m_position = pos;
		//バネカメラに注視点と視点を設定する。
		m_springCamera.SetPosition(pos);
		m_springCamera.SetTarget(target);
		m_springCamera.Update();
	}
	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
private:
	Vector3 m_toCameraPos;			//注視点から視点に向かうベクトル。
	Vector3 m_position;
	Vector3 m_cameraVelocity;
	Vector3 m_targetVelocity;
	Vector3 m_Target;				//注視点。
	bool m_TargetPlayer = true;		//基本の注視点はプレイヤーか指定した座標か。
	Quaternion m_quaternion;
	Game* m_game;
	Player* m_player;
	SpringCamera m_springCamera;	//ばねカメラ。
	Vector3 m_targetPosition;		//向かう座標。
	bool m_cameraMove = false;
	bool m_targetMove = false;
	bool m_reverse = false;

	bool m_isTargetMoving = true;
	bool m_isCameraMoving = true;
	bool m_movieCamera = false;
};
