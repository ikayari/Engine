#pragma once
class Slow:public IGameObject
{
public:
	Slow() {};
	~Slow() {};
	bool Start();
	void Update();

	void StartSlowMotion(float timer = 0.0f)
	{
		m_isSlowMotion = true;
		m_SlowRatio = 0.25f;
		m_timer = 0.0f;
		m_Endtimer = timer;
	}
	void EndSlowMotion()
	{
		m_isSlowMotion = false;
		m_SlowRatio = 1.0f;
	}
	const bool& IsSlow()
	{
		return m_isSlowMotion;
	}
	const float& GetSlowRatio()const
	{
		return m_SlowRatio;
	}
private:
	bool  m_isSlowMotion = false;
	float m_SlowRatio=1.0f;
	float m_timer = 0.0f;
	float m_Endtimer = 0.0f;
};

