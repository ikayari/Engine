#pragma once
#include <SpringCamera.h>

class Game;
class Player;
//�Q�[�����̃J�����𐧌䂷��B
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
	/// �J�����̈ړ�����ʒu
	/// </summary>
	/// <param name="pos">���W</param>
	void SetCameraPosition(Vector3& pos)
	{
		m_targetPosition = pos;
	}
	/// <summary>
	/// �ڎw�������_�̈ʒu
	/// </summary>
	/// <param name="pos">���W</param>
	void SetTarget(Vector3& pos)
	{
		m_Target = pos;
	}
	/// <summary>
	/// �J���������������ē���
	/// </summary>
	/// <param name="b"></param>
	void SetCameraMove(bool b)
	{
		m_cameraMove = b;
	}
	/// <summary>
	/// �����_�����������ē����B
	/// </summary>
	/// <param name="b"></param>
	void SetTargetMove(bool b)
	{
		m_targetMove = b;
	}
	/// <summary>
	/// �����_���v���C���[�ɂ���H
	/// </summary>
	/// <param name="b"></param>
	void SetTargetPlayer(bool b)
	{
		m_TargetPlayer = b;
	}
	/// <summary>
	/// �ʏ�ɖ߂�
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
		//�����_���v�Z����B
		Vector3 target = targetPos;
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target.y += 80.0f;
		target += g_camera3D->GetForward() * 20.0f;
		//���_���v�Z����B
		Vector3 pos = target + m_toCameraPos;
		m_position = pos;
		//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
		m_springCamera.SetPosition(pos);
		m_springCamera.SetTarget(target);
		m_springCamera.Update();
	}
	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
private:
	Vector3 m_toCameraPos;			//�����_���王�_�Ɍ������x�N�g���B
	Vector3 m_position;
	Vector3 m_cameraVelocity;
	Vector3 m_targetVelocity;
	Vector3 m_Target;				//�����_�B
	bool m_TargetPlayer = true;		//��{�̒����_�̓v���C���[���w�肵�����W���B
	Quaternion m_quaternion;
	Game* m_game;
	Player* m_player;
	SpringCamera m_springCamera;	//�΂˃J�����B
	Vector3 m_targetPosition;		//���������W�B
	bool m_cameraMove = false;
	bool m_targetMove = false;
	bool m_reverse = false;

	bool m_isTargetMoving = true;
	bool m_isCameraMoving = true;
	bool m_movieCamera = false;
};
