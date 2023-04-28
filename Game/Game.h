#pragma once
#include "Player.h"
#include <physics/PhysicsGhostObject.h>
#include "GameCamera.h"

class EffectManage;
class Slow;
class Sound;
class Stage;
class FinalWaveMovie;
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
	void AddEnemy()
	{
		EnemyCount++;
	};
private:
	ModelRender		m_modelRender;
	ModelRender		m_modelRender2;
	ModelRender     SkyCube;
	ModelRender     m_backGround;
	ModelRender     m_backGroundBuilding;
	ModelRender     m_box;
	Quaternion		m_rotation;
	PhysicsStaticObject PSO;

	Player* m_player;

	
	GameCamera* m_gameCamera;

	Slow* m_slow;
	EffectManage* m_Effect;

	PhysicsGhostObject m_PGO;

	EffectEmitter* m_effectEmitter;

	FinalWaveMovie* m_finalWaveMovie;

	LevelRender					m_levelRender;
	LevelRender					m_levelRender2;
	Sound* m_sound;
	Stage* m_stage;
	int EnemyCount = 0;

	int m_wave = 0;
	bool m_isWaitFadeout;
};

