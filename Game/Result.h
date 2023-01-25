#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

class Game;

class Result :public IGameObject
{
public:
	bool Start();
	Result() {};
	~Result();

	void Update();
	void Render(RenderContext& rc);


private:
	SpriteRender sprite;
	SoundSource* m_bgm = nullptr;
	Game* m_game = nullptr;
	bool m_isWaitFadeout = false;
};

