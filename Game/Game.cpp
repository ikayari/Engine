#include "stdafx.h"
#include "Game.h"
#include "Slow.h"
#include "Cannon.h"



Game::~Game()
{

}

bool Game::Start()
{
	m_backGround.SetRecieveShadow(true);
	//m_backGround.SetOutLineDraw(true);
	m_backGround.Init("Assets/modelData/Stage/Stage.tkm");
	
	m_box.Init("Assets/modelData/Stage/box.tkm");
	m_box.SetClip(0.5f);
	//PSO.CreateFromModel(m_backGround.GetModel(),m_backGround.GetModel().GetWorldMatrix());
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_player = NewGO<Player>(0, "player");
	//hogeho=NewGO<Player>(0, "layer");
	m_gameCamera = NewGO<GameCamera>(0, "camera");
	SkyCube.Init("Assets/modelData/debug/skycube.tkm");
	SkyCube.SetScale({ 0.1f,0.1f,0.1f });
	SkyCube.Update();
	//m_PGO.CreateBox({ 0.0f,50.0f,0.0f }, Quaternion::Identity, Vector3::One*100.0f);
	//コリジョンオブジェクトを作成する。
	//m_CO = NewGO<CollisionObject>(0);

	//
	//m_CO->CreateBox({ 0.0f,50.0f,0.0f },               //座標。
	//	Quaternion::Identity,                                       //回転。
	//	Vector3::One * 100.0f                            //大きさ。
	//);
	//m_CO->SetName("player_attack");
	//m_CO->SetIsEnableAutoDelete(false);

	m_slow = FindGO<Slow>("slow");
	
	m_cannon = NewGO<Cannon>(0, "cannon");
	m_cannon->SetPosition({ 0.0f, 0.0f,500.0f });
	Quaternion rot;
	rot.AddRotationDegY(360.0f);
	m_cannon->SetRotation(rot);
	m_cannon->SetScale(Vector3::One * 4.0f);
	return true;
}

void Game::Update()
{
	//g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable,60.0f);
	/*
	if (g_pad[0]->IsPress(enButtonSelect))
	{
		m_slow->StartSlowMotion();
		//g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 5.0f);
	}
	else
	{
		m_slow->EndSlowMotion();
	}
	*/
	hogehoge.AddRotationDegY(10.0f);
	hogehoge.Apply(hoge);
	hoge.Normalize();

}
void Game::Render(RenderContext& rc)
{
	//SkyCube.Draw(rc);

	m_backGround.Draw(rc);
	//m_box.Draw(rc);

}