#include "stdafx.h"
#include "Title.h"
#include "Game.h"


bool Title::Start()
{
	g_fade.StartFadeIn();
	sprite.Init("Assets/sprite/Title/title.dds", 1600, 900);
	/*g_soundEngine->ResistWaveFileBank(100, "Assets/sound/BGM/You_and_Me.wav");
	g_soundEngine->ResistWaveFileBank(250, "Assets/sound/SE/kettei.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(100);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.1f);*/

	return true;
}
Title::~Title()
{
	//DeleteGO(m_bgm);
}
void Title::Update()
{

	if (m_isWaitFadeout) {
		if (!g_fade.IsFade()) {
			NewGO<Game>(0, "game");
			//���g���폜����B
			DeleteGO(this);
		}
	}
	else {
		//A�{�^������������B
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			g_fade.StartFadeOut();
		}
	}

}
void Title::Render(RenderContext& rc)
{
	sprite.Draw(rc);

}