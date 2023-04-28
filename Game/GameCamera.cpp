#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "Game.h"
// 目標点との距離がこの値よりも小さくなったら停止する
const float TARGET_RADIUS = 10.0f;
// 速度がこの値よりも小さくなったら停止する
const float STOP_SPEED = 0.1f;
// 最大速度
const float MAX_SPEED = 2000.0f;
// 加速度
const float ACCELERATION = 500.0f;
// 減速度
const float DECELERATION = 1500.0f;

GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 150.0f, -250.0f);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);
	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		2000.0f			//カメラの移動速度の最大値。
		);
	m_quaternion = m_player->GetRotation();
	m_quaternion.Apply(m_toCameraPos);
	//g_camera3D->SetViewAngle(0.4f);
	return true;
}
void GameCamera::Update()
{
	ProcessNormal();
	ProcessTarget();
	m_springCamera.Update();
}

void GameCamera::ProcessNormal()
{

	//カメラ移動
	if (m_cameraMove||m_targetMove||m_movieCamera)
	{
		return;
	}
	m_springCamera.SetDampingRate(0.001f);
	//カメラを更新。
	//注視点を計算する。
	Vector3 target = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 80.0f;
	target += g_camera3D->GetForward() * 20.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 3.0f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 3.0f * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;
	m_position = pos;
	//バネカメラに注視点と視点を設定する。
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);		
}

void GameCamera::ProcessTarget()
{
	CameraMove();
	TargetMove();
}

void GameCamera::CameraMove()
{
	
	//カメラ移動
	if (!m_cameraMove)
	{
		m_cameraVelocity = Vector3(0, 0, 0);
		return;
	}
	m_isCameraMoving = true;
	Vector3 direction = m_targetPosition - m_springCamera.GetPosition();
	// 目標点に到着したら停止する
	if (direction.Length() < TARGET_RADIUS) {
		m_cameraVelocity = Vector3(0, 0, 0);
		m_isCameraMoving = false;
		return;
	}
	// 加速または減速する
	float speed = m_cameraVelocity.Length();
	float speedLimit = MAX_SPEED;
	direction.Normalize();
	Vector3 acceleration = direction * ACCELERATION;
	if (speed > speedLimit) {
		acceleration *= -1.0f * DECELERATION;
	}

	m_cameraVelocity = m_cameraVelocity + acceleration * g_gameTime->GetFrameDeltaTime();
	// 速度が小さくなったら停止する
	if (m_cameraVelocity.Length() < STOP_SPEED) {
		m_cameraVelocity = Vector3(0, 0, 0);
	}
	// 移動する
	m_springCamera.SetPosition(m_springCamera.GetPosition() + m_cameraVelocity * g_gameTime->GetFrameDeltaTime());
}

void GameCamera::TargetMove()
{

	//カメラ移動
	//注視点を加減速移動させないとき。
	if (!m_targetMove)
	{
		m_targetVelocity = Vector3(0, 0, 0);
		if (m_TargetPlayer)
		{
			//注視点を計算する。
			m_Target = m_player->GetPosition();
			//プレイヤの足元からちょっと上を注視点とする。
			m_Target.y += 80.0f;
			m_Target += g_camera3D->GetForward() * 20.0f;
		}
		m_springCamera.SetTarget(m_Target);
		return;
	}
	//注視点がプレイヤーか
	if (m_TargetPlayer)
	{
		//注視点を計算する。
		m_Target = m_player->GetPosition();
		//プレイヤの足元からちょっと上を注視点とする。
		m_Target.y += 80.0f;
		m_Target += g_camera3D->GetForward() * 20.0f;
	}
	m_isTargetMoving = true;
	Vector3 direction = m_Target - m_springCamera.GetTarget();
	// 目標点に到着したら停止する
	if (direction.Length() < TARGET_RADIUS) {
		m_targetVelocity = Vector3(0, 0, 0);
		m_isTargetMoving = false;
		return;
	}
	// 加速または減速する
	float speed = m_targetVelocity.Length();
	float speedLimit = MAX_SPEED;
	direction.Normalize();
	Vector3 acceleration = direction * ACCELERATION;
	if (speed > speedLimit) {
		acceleration *= -1.0f * DECELERATION;
	}

	m_targetVelocity = m_targetVelocity + acceleration * g_gameTime->GetFrameDeltaTime();
	// 速度が小さくなったら停止する
	if (m_targetVelocity.Length() < STOP_SPEED) {
		m_targetVelocity = Vector3(0, 0, 0);
	}
	// 移動する
	m_springCamera.SetTarget(m_springCamera.GetTarget() + m_targetVelocity * g_gameTime->GetFrameDeltaTime());
}

