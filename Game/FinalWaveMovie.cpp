#include "stdafx.h"
#include "FinalWaveMovie.h"
#include "Spider.h"
#include "ShootEnemy.h"
#include "Game.h"
#include "PlayerUI.h"
#include "Player.h"
#include "GameCamera.h"


FinalWaveMovie::~FinalWaveMovie()
{
}

bool FinalWaveMovie::Start()
{
	m_game=FindGO<Game>("game");
	m_camera = FindGO<GameCamera>("camera");
	return true;
}

void FinalWaveMovie::Update()
{
	if (m_playMovie)
	{
		m_camera->SetMovieCamera(true);
		PlayerUI* playerui = FindGO<PlayerUI>("playerUI");
		playerui->DisableUI();
		Player* player = FindGO<Player>("player");
		player->EnableMovieMode();
		if (!m_doOnce_CreateLevel)
		{
			m_levelRender.Init("Assets/level/EnemyLevel3.tkl",
				[&](LevelObjectData& objData)
				{
					if (objData.EqualObjectName(L"spider") == true)
					{
						auto spider = NewGO<Spider>(0, "spider");
						spider->SetPosition(objData.position);
						spider->SetRotation(objData.rotation);
						spider->SetScale(objData.scale);
						m_game->AddEnemy();
						spider->EnableMovieMode();
						spider->SetModelDraw(false);
						return true;
					}
					else if (objData.EqualObjectName(L"ShootEnemy"))
					{
						m_shootEnemy = NewGO <ShootEnemy>(0, "shootEnemy");
						m_shootEnemy->SetPosition(objData.position);
						m_shootEnemy->SetRotation(objData.rotation);
						m_shootEnemy->SetScale(objData.scale);
						m_shootEnemy->EnableMovieMode();
						m_game->AddEnemy();
						return true;
					}
					else if (objData.EqualObjectName(L"TargetPosition"))
					{
						m_TargetPosition = objData.position;
						return true;
					}
					else if (objData.EqualObjectName(L"FinalPosition"))
					{
						m_FinalPosition = objData.position;
						return true;
					}
			
				});
			
			m_doOnce_CreateLevel = true;
			m_moveFirstTarget = true;
		}
		if (m_moveFirstTarget)
		{
			m_camera->SetTargetPlayer(false);
			m_camera->SetTargetMove(true);
			m_camera->SetTarget(m_TargetPosition);
			m_shootEnemy->SetMovePosition(m_TargetPosition);
			m_shootEnemy->MoveCharaToPosition(10.0f);

			Vector3 distance = m_shootEnemy->GetPosition() - m_TargetPosition;
			if (!m_camera->isMovingTarget()&&distance.Length()<=30.0f)
			{
				m_moveFirstTarget = false;
				m_moveSecondTarget = true;
			}
		}
		if (m_moveSecondTarget)
		{
			Vector3 cameraTarget = m_shootEnemy->GetPosition();
			m_camera->SetTarget(cameraTarget);
			m_camera->SetTargetMove(false);
			m_shootEnemy->SetMovePosition(m_FinalPosition);
			m_shootEnemy->MoveCharaToPosition(10.0f);
			Vector3 distance = m_shootEnemy->GetPosition() - m_FinalPosition;
			if (distance.Length()<= 5.0f)
			{
				m_moveSecondTarget = false;
				m_moveThirdTarget = true;
				m_camera->ResetTargetMoveVelocity();
			}
		}
		if (m_moveThirdTarget)
		{
			
			if (!m_camera->isMovingTarget())
			{
				m_moveThirdTarget = false;
				m_spiderFall = true;
				m_camera->ResetTargetMoveVelocity();				
			}
			
		}
		if (m_spiderFall)
		{
			if (!m_doOnce_SpiderUseGravity)
			{
				const auto& spiders = FindGOs<Spider>("spider");
				for (auto spider : spiders)
				{
					spider->SetUseGravity(true);
					spider->SetModelDraw(true);
					m_spider = spider;
				}
				m_doOnce_SpiderUseGravity = true;
			}
			if (m_spider->GetCharacterController().IsOnGround())
			{
				m_spiderFall = false;
				m_movieEndTimerStart = true;
				Vector3 direction = m_FinalPosition - player->GetPosition();
				m_camera->SetCameraRotateDirection(direction,player->GetPosition());
				m_camera->SetTargetMove(false);
			}

		}
		if (m_movieEndTimerStart)
		{
			m_timer += g_gameTime->GetFrameDeltaTime();
			if (m_timer >= 5.0f)
			{
				m_camera->SetTargetPlayer(true);
				m_isEnd = true;
				m_playMovie = false;
				playerui->EnableUI();
				m_shootEnemy->DisableMovieMode();
				player->DisableMovieMode();
				const auto& spiders = FindGOs<Spider>("spider");
				for (auto spider : spiders)
				{
					spider->DisableMovieMode();
				}
				m_camera->SetMovieCamera(false);
				
			}
		}
		
	}
}
