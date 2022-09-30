#pragma once

class Game : public IGameObject
{
public:
	//////////////////////////////////////
	// ƒƒ“ƒoŠÖ”B
	//////////////////////////////////////
	Game() {};
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	////////////////////////////////////
	// ƒƒ“ƒo•Ï”‚Æ‚©
	////////////////////////////////////
private:
	ModelRender		m_modelRender;
	ModelRender		m_modelRender2;
	ModelRender     SkyCube;
	ModelRender     m_backGround;
	Quaternion		m_rotation;

};

