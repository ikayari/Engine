#pragma once
class Game;
class ShootEnemy;
class Spider;
class GameCamera;
class FinalWaveMovie:public IGameObject
{
public:
	~FinalWaveMovie();
	bool Start();
	void Update();
	void Draw(RenderContext& rc);

	void PlayMovie()
	{
		m_playMovie = true;
	}
private:
	float m_timer;
	Vector3 m_position;
	
	bool m_isEnd = false;

	Game* m_game;

	bool m_playMovie = false;
	bool m_doOnce_CreateLevel = false;
	bool m_doOnce_SpiderUseGravity = false;

	const std::vector<Spider*> m_spiders;
	ShootEnemy* m_shootEnemy;
	Spider* m_spider;
	Vector3 m_TargetPosition;
	Vector3 m_FinalPosition;
	LevelRender m_levelRender;

	GameCamera* m_camera;

	bool m_moveFirstTarget = false;
	bool m_moveSecondTarget = false;
	bool m_movieEndTimerStart = false;
	bool m_moveThirdTarget = false;
	bool m_spiderFall = false;
};

