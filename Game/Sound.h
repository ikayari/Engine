#pragma once
class Sound:public IGameObject
{
public:
	Sound() {};
	~Sound();
	bool Start();
	void Update();

	SoundSource* PlaySE(int num,float vol=1.0f, bool roop = false)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num);
		se->Play(roop);
		se->SetVolume(vol);
		return se;
	}
	SoundSource* PlaySE3D(int num, Vector3 pos, float vol = 1.0f, bool roop = false)
	{
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(num,true);
		se->Play(roop);
		se->SetVolume(vol);
		se->SetPosition(pos);
		return se;
	}
	void PlayBGM(int num, float vol = 1.0f)
	{
		if (m_BGM != nullptr)
		{
			DeleteGO(m_BGM);
		}
		m_BGM = NewGO<SoundSource>(0);
		m_BGM->Init(num);
		m_BGM->Play(true);
		m_BGM->SetVolume(0.0f);
		m_BGMVolume = vol;
		
	}
	void StopBGM()
	{
		if (m_BGM != nullptr)
		{
			m_BGM->Pause();
		}
	}
	SoundSource* GetBGM()
	{
		return m_BGM;
	}

	SoundSource* m_BGM;
	float m_BGMVolume;
};

