#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Slow.h"
#include "Sound.h"
#include "EffectManage.h"
#include "Title.h"
#include "Result.h"

// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("GAME"));

	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_sceneLight.SeteyePosition(g_camera3D->GetPosition());

	g_renderingEngine.Init();
	g_postEffect.Init();
	g_fade.Init();
	CollisionObjectManager collisionObjectManager;
	g_collisionObjectManager = &collisionObjectManager;
	
	NewGO<Sound>(0, "sound");
	NewGO<EffectManage>(0, "effectManage");
	NewGO<Slow>(0, "slow");
	NewGO<Title>(0, "title");
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();

		//g_engine->ExecuteRender();

		g_renderingEngine.Execute(renderContext);



		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}
	delete g_k2EngineLow;

	return 0;
}

