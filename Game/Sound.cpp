#include "stdafx.h"
#include "Sound.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Sound::~Sound()
{
	if (!m_BGM->IsDead())
	{
		DeleteGO(m_BGM);
	}
}
bool Sound::Start()
{
	g_soundEngine->ResistWaveFileBank(1, "Assets/Sound/SE/fire.wav");
	g_soundEngine->ResistWaveFileBank(2, "Assets/Sound/SE/ShootEnemy/Idle.wav");
	g_soundEngine->ResistWaveFileBank(100, "Assets/Sound/BGM/Game.wav");
	g_soundEngine->ResistWaveFileBank(101, "Assets/Sound/BGM/Title.wav");
	g_soundEngine->ResistWaveFileBank(102, "Assets/Sound/BGM/GameOver.wav");
	g_soundEngine->ResistWaveFileBank(103, "Assets/Sound/BGM/GameClear.wav");
	return true;
}
void Sound::Update()
{
	if (m_BGM!=nullptr)
	{
		m_BGM->SetVolume(m_BGMVolume * (g_fade.GetCurrentAlpha()-1.0f));
	}
}