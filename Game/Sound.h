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
	void PlaySE3D(int num, Vector3 pos, float vol = 1.0f, bool roop = false)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num,true);
		se->Play(roop);
		se->SetVolume(vol);
		se->SetPosition(pos);
	}
};

