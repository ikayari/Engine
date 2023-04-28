#include "stdafx.h"
#include "Result.h"
#include "Title.h"
#include "Sound.h"



bool Result::Start()
{

	m_sound = FindGO<Sound>("sound");
	switch (m_result)
	{
	case enResultType_GameClear:
		m_spriteRender.Init("Assets/sprite/Result/GameClear.dds", 1600, 900);
		m_sound->StopBGM();
		m_sound->PlaySE(103, 0.1f);
		break;
	case enResultType_GameOver:
		m_spriteRender.Init("Assets/sprite/Result/GameOver.dds", 1600, 900);
		m_sound->PlayBGM(102, 0.5f);
		break;
	default:
		break;
	}
	

	g_fade.StartFadeIn();
	return true;
}
Result::~Result()
{
	
}
void Result::Update()
{
	if (m_isWaitFadeout) {
		if (!g_fade.IsFade()) {
			NewGO<Title>(0, "title");
			//自身を削除する。
			DeleteGO(this);
		}
	}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			g_fade.StartFadeOut();
		}
	}

}
void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);

}