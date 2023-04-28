#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Sound.h"


bool Title::Start()
{
	
	g_fade.StartFadeIn();
	sprite.Init("Assets/sprite/Title/title.dds", 1600, 900);
		
	m_sound = FindGO<Sound>("sound");
	m_sound->PlayBGM(101,0.5f);
	return true;
}
Title::~Title()
{
	
}
void Title::Update()
{

	if (m_isWaitFadeout) {
		if (!g_fade.IsFade()) {
			NewGO<Game>(0, "game");
			//自身を削除する。
			DeleteGO(this);
		}
	}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			g_fade.SetFadeImage(enFadeImage_Loading);
			g_fade.StartFadeOut();
		}
	}

}
void Title::Render(RenderContext& rc)
{
	sprite.Draw(rc);

}