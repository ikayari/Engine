#include "stdafx.h"
#include "Sound.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Sound::~Sound()
{

}
bool Sound::Start()
{
	g_soundEngine->ResistWaveFileBank(1, "Assets/Sound/fire.wav");
	return true;
}
void Sound::Update()
{

}