#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;
	void RenderingEngine::Render2DDraw(RenderContext& rc)
	{
		for (auto& renderObj : m_renderobject) {
			renderObj->OnRenderSprite2D(rc);
		}
		for (auto& renderObj : m_renderobject) {
			renderObj->OnRenderFont2D(rc);
		}
	}

	void RenderingEngine::ShadowMapDraw(RenderContext& rc)
	{
		Vector3 dir = { g_sceneLight.GetDirectionLightDirection(0).x,g_sceneLight.GetDirectionLightDirection(0).y,g_sceneLight.GetDirectionLightDirection(0).z };
		m_shadowMapRender.Render(rc, dir, m_renderobject);


	}
	void RenderingEngine::Execute(RenderContext& rc)
	{
		m_modelRenderCB.m_light = g_sceneLight.GetLight();
		m_modelRenderCB.m_lvp = GetLightCamera().GetViewProjectionMatrix();
		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		m_fade->Update();
		
		DrawModelAndDepth(rc);
		EffectEngine::GetInstance()->Draw();
		ShadowMapDraw(rc);

		

		m_postEffect->Render(rc);

		DrawOutLine(rc);

		SpriteInitData luminanceSpriteInitData;

		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/OutLine_PostEffect.fx";

		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";

		luminanceSpriteInitData.m_psEntryPoinFunc = "PSMain";

		luminanceSpriteInitData.m_width = 1600;
		luminanceSpriteInitData.m_height = 900;

		luminanceSpriteInitData.m_textures[0] = &g_renderingEngine.GetDepthRenderTarget().GetRenderTargetTexture();
		luminanceSpriteInitData.m_textures[1] = &g_renderingEngine.GetdepthOutLineRenderTarget().GetRenderTargetTexture();
		//描き込むレンダリングターゲットのフォーマットを指定する。
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//作成した初期化情報をもとにスプライトを初期化する。
		luminanceSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
		Sprite sprite;
		sprite.Init(luminanceSpriteInitData);
		sprite.Draw(rc);
		Render2DDraw(rc);

		m_fade->Render(rc);
		m_renderobject.clear();
	}
	void RenderingEngine::DrawOutLine(RenderContext& rc)
	{
		//レンダリングターゲットとして利用できるまで待つ
		rc.SetRenderTarget(
			g_renderingEngine.GetdepthOutLineRenderTarget().GetRTVCpuDescriptorHandle(),
			g_renderingEngine.GetmainRenderTarget().GetDSVCpuDescriptorHandle()
		);
		rc.ClearRenderTargetView(g_renderingEngine.GetdepthOutLineRenderTarget());
		
			for (auto& renderObj : m_renderobject) {
			renderObj->OnRenderOutLineModel(rc);
		}
		rc.WaitUntilFinishDrawingToRenderTarget(g_renderingEngine.GetdepthOutLineRenderTarget());
		// 通常レンダリング
		// レンダリングターゲットをフレームバッファに戻す
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	}
	void RenderingEngine::DrawModelAndDepth(RenderContext& rc)
	{
		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		RenderTarget* rts[] = {
			&g_renderingEngine.GetmainRenderTarget(),
			&g_renderingEngine.GetDepthRenderTarget(),
			&g_renderingEngine.GetvelocityRenderTarget()
		};

		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(3, rts);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetsAndViewport(3, rts);
		// レンダリングターゲットをクリア
		rc.ClearRenderTargetViews(3, rts);
		//モデルをドロー。
		g_engine->ExecuteRender();
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTargets(3, rts);
	
	}
	void RenderingEngine::Init()
	{
		m_postEffect = &g_postEffect;
		m_shadowMapRender.Init();
		m_modelRenderCB.m_light = g_sceneLight.GetLight();
		m_modelRenderCB.m_lvp = GetLightCamera().GetViewProjectionMatrix();
		m_fade = &g_fade;
		//RenderTarget::Create()を利用して、レンダリングターゲットを作成する。
		m_mainRenderTarget.Create(
			1600,												//テクスチャの幅。
			900,												//テクスチャの高さ。
			1,													//Mipmapレベル。
			1,													//テクスチャ配列のサイズ。
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT				//デプスステンシルバッファのフォーマット。
		);
		m_depthRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		m_velocityRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		m_depthOutLineRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
		m_normalRenderTarget.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);
	}
}