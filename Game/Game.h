#pragma once
#include "Player.h"
#include <physics/PhysicsGhostObject.h>
#include "GameCamera.h"

class EffectManage;
class Slow;
class ShootEnemy;
class Spider;
class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// ƒƒ“ƒoŠÖ”B
	//////////////////////////////////////
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	////////////////////////////////////
	// ƒƒ“ƒo•Ï”‚Æ‚©
	////////////////////////////////////
	void subEnemy()
	{
		EnemyCount--;
	};
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
	ShootEnemy* m_shootEnemy;
	GameCamera* m_gameCamera;
	Spider* m_spider;
	Spider* m_spider1;
	Spider* m_spider2;
	Spider* m_spider3;

	Slow* m_slow;
	EffectManage* m_Effect;

	PhysicsGhostObject m_PGO;
	CollisionObject* m_CO;

	EffectEmitter* m_effectEmitter;
	FontRender font;

	LevelRender					m_levelRender;
	LevelRender					m_levelRender2;


	int EnemyCount = 0;

	int m_wave = 0;
	bool m_isWaitFadeout;
};

