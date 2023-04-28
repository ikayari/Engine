#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

class Game;
class Sound;

class Result :public IGameObject
{
public:
	enum ResultType
	{
		enResultType_GameOver,
		enResultType_GameClear
	};
	bool Start();
	Result() {};
	~Result();

	void Update();
	void Render(RenderContext& rc);
	void SetResultType(ResultType type)
	{
		m_result = type;
	}

private:
	SpriteRender m_spriteRender;


	SoundSource* m_bgm = nullptr;
	Game* m_game = nullptr;
	bool m_isWaitFadeout = false;


	ResultType m_result=enResultType_GameClear;
	Sound* m_sound;
};

