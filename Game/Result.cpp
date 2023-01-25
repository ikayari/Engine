#include "stdafx.h"
#include "Result.h"
#include "Title.h"



bool Result::Start()
{
	g_fade.StartFadeIn();
	sprite.Init("Assets/sprite/Result/GameOver.dds", 1600, 900);
	/*g_soundEngine->ResistWaveFileBank(100, "Assets/sound/BGM/You_and_Me.wav");
	g_soundEngine->ResistWaveFileBank(250, "Assets/sound/SE/kettei.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(100);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.1f);*/

	return true;
}
Result::~Result()
{
	//DeleteGO(m_bgm);
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
	sprite.Draw(rc);

}