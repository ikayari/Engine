#include "stdafx.h"
#include "Game.h"




Game::~Game()
{

}

bool Game::Start()
{
	m_backGround.SetRecieveShadow(true);
	//m_backGround.SetOutLineDraw(true);
	m_backGround.Init("Assets/modelData/debug/ground.tkm");
	
	m_modelRender.SetCasterShadow(true);
	m_modelRender.SetOutLineDraw(true);
	m_modelRender.Init("Assets/modelData/unityChan.tkm");
	m_modelRender2.SetDithering(en_dithering);
	m_modelRender2.Init("Assets/modelData/unityChan.tkm");
	m_modelRender2.SetRotation(m_rotation);
	m_modelRender2.Update();
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

	SkyCube.Init("Assets/modelData/debug/skycube.tkm");
	SkyCube.SetScale({ 0.1f,0.1f,0.1f });
	SkyCube.Update();
	return true;
}

void Game::Update()
{
	m_rotation.AddRotationDegY(1.0f);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();
	m_modelRender2.SetRotation(m_rotation);
	m_modelRender2.Update();
}
void Game::Render(RenderContext& rc)
{
	SkyCube.Draw(rc);

	m_modelRender.Draw(rc);
	m_backGround.Draw(rc);

}