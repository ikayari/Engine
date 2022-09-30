#pragma once

class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// �����o�֐��B
	//////////////////////////////////////
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	////////////////////////////////////
	// �����o�ϐ��Ƃ�
	////////////////////////////////////
private:
	ModelRender		m_modelRender;
	ModelRender		m_modelRender2;
	ModelRender     SkyCube;
	ModelRender     m_backGround;
	Quaternion		m_rotation;

};

