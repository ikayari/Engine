#pragma once
#include "Player.h"
#include <physics/PhysicsGhostObject.h>
#include "GameCamera.h"

class Slow;
class Cannon;
class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// �����o�֐��B
	//////////////////////////////////////
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	////////////////////////////////////
	// �����o�ϐ��Ƃ�
	////////////////////////////////////
private:
	ModelRender		m_modelRender;
	ModelRender		m_modelRender2;
	ModelRender     SkyCube;
	ModelRender     m_backGround;
	ModelRender     m_box;
	Quaternion		m_rotation;
	PhysicsStaticObject PSO;

	Vector3 hoge;
	Quaternion hogehoge;

	Player* m_player;
	Player* hogeho;
	Cannon* m_cannon;
	GameCamera* m_gameCamera;

	Slow* m_slow;

	PhysicsGhostObject m_PGO;
	CollisionObject* m_CO;

};

