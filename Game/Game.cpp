#include "stdafx.h"
#include "Game.h"
#include "Slow.h"
#include "Cannon.h"
#include "ShootEnemy.h"
#include "Spider.h"
#include "EffectManage.h"
#include "Result.h"
#include "Sound.h"
#include "Stage.h"
#include "FinalWaveMovie.h"

Game::~Game()
{

	DeleteGO(m_player);
	const auto& spider = FindGOs<Spider>("spider");
	for (auto spiders : spider)
	{
		DeleteGO(spiders);
	}
	const auto& shootenemy = FindGOs<ShootEnemy>("shootEnemy");
	for (auto shootenemys : shootenemy)
	{
		DeleteGO(shootenemys);
	}
	DeleteGO(m_gameCamera);
}

bool Game::Start()
{	
	m_finalWaveMovie = NewGO<FinalWaveMovie>(0, "finalWaveMovie");
	m_levelRender.Init("Assets/level/PlayerLevel.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"spider") == true)
			{
				auto spider = NewGO<Spider>(0, "spider");
				spider->SetPosition(objData.position);
				spider->SetRotation(objData.rotation);
				spider->SetScale(objData.scale);
				EnemyCount++;
				
				return true;
			}
			else if (objData.EqualObjectName(L"Player") == true)
			{
				m_player = NewGO<Player>(0, "player");
				m_player->SetPosition(objData.position);
				m_player->SetRotation(objData.rotation);
				m_player->SetScale(objData.scale);
				return true;
			}
			else if (objData.EqualObjectName(L"stage"))
			{
				m_stage = NewGO<Stage>(0, "stage");
				m_stage->SetPosition(objData.position);
				m_stage->SetRotation(objData.rotation);
				m_stage->SetScale(objData.scale);
				return true;
			}
			else if (objData.EqualObjectName(L"ShootEnemy"))
			{
				auto shootEnemy = NewGO < ShootEnemy >(0, "shootEnemy");
				shootEnemy->SetPosition(objData.position);
				shootEnemy->SetRotation(objData.rotation);
				shootEnemy->SetScale(objData.scale);
				EnemyCount++;
				return true;
			}
	
		});
	m_wave += 2;
	m_sound = FindGO<Sound>("sound");
	m_sound->PlayBGM(100, 0.05f);
	g_fade.StartFadeIn();
	
	
	m_backGround.SetRecieveShadow(true);


	
	m_box.Init("Assets/modelData/Stage/box.tkm");
	m_box.SetClip(0.5f);
	
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	
	m_gameCamera = NewGO<GameCamera>(0, "camera");
	SkyCube.Init("Assets/modelData/debug/skycube.tkm");
	SkyCube.SetScale({ 0.1f,0.1f,0.1f });
	SkyCube.Update();
	


	m_slow = FindGO<Slow>("slow");
	m_Effect = FindGO<EffectManage>("effectManage");
	
	return true;
}

void Game::Update()
{
	
	if (EnemyCount == 0)
	{
		if (m_wave == 1)
		{
			
			m_levelRender2.Init("Assets/level/EnemyLevel.tkl",
				[&](LevelObjectData& objData)
				{
					if (objData.EqualObjectName(L"spider") == true)
					{
						auto spider = NewGO<Spider>(0, "spider");
						spider->SetPosition(objData.position);
						spider->SetRotation(objData.rotation);
						spider->SetScale(objData.scale);
						EnemyCount++;

						return true;
					}
					else if (objData.EqualObjectName(L"ShootEnemy"))
					{
						auto shootEnemy = NewGO < ShootEnemy >(0, "shootEnemy");
						shootEnemy->SetPosition(objData.position);
						shootEnemy->SetRotation(objData.rotation);
						shootEnemy->SetScale(objData.scale);
						EnemyCount++;
						return true;
					}
				});

		}
		else if (m_wave == 2)
		{
			m_finalWaveMovie->PlayMovie();
		}
		else
		{
			//ウェーブを全部クリアしたらゲームクリア
			if (m_isWaitFadeout) {
				if (!g_fade.IsFade()) {
					NewGO<Result>(0, "result")->SetResultType(Result::enResultType_GameClear);

					//自身を削除する。
					DeleteGO(this);
				}
			}
			else {
				m_isWaitFadeout = true;
				g_fade.SetFadeImage(enFadeImage_None);
				g_fade.StartFadeOut();

			}
			

		}
		m_wave++;
	}
	//プレイヤーがダウンしたらゲームオーバー
	if (m_player->GetPlayerState() == m_player->enPlayerState_Down)
	{
		if (m_isWaitFadeout) {
			if (!g_fade.IsFade()) {
				NewGO<Result>(0, "result")->SetResultType(Result::enResultType_GameOver);;
				//自身を削除する。
				DeleteGO(this);
			}
		}
		else {
			m_isWaitFadeout = true;
			g_fade.SetFadeImage(enFadeImage_None);
			g_fade.StartFadeOut();

		}
	};
}
void Game::Render(RenderContext& rc)
{
	//SkyCube.Draw(rc);

	m_backGround.Draw(rc);
	m_backGroundBuilding.Draw(rc);

}