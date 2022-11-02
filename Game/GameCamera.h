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

	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
private:
	Player* m_Player;	//�v���C���[�B
	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	Quaternion m_quaternion;
	Game* m_game;
	Player* m_player;
	SpringCamera m_springCamera;	//�΂˃J�����B

};
