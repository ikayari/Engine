#pragma once
class Sound:public IGameObject
{
public:
	Sound() {};
	~Sound();
	bool Start();
	void Update();

	void PlaySE(int num,float vol=1.0f, bool roop = false)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(roop);
		se->SetVolume(vol);
	}
};

