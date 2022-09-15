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
		Vector3 dir = { g_sceneLight.GetDirectionLightDirection().x,g_sceneLight.GetDirectionLightDirection().y,g_sceneLight.GetDirectionLightDirection().z };
		m_shadowMapRender.Render(rc, dir, m_renderobject);


	}
	void RenderingEngine::Execute(RenderContext& rc)
	{
		m_modelRenderCB.m_light = g_sceneLight.GetLight();
		m_modelRenderCB.m_lvp = GetLightCamera().GetViewProjectionMatrix();
		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		
		ShadowMapDraw(rc);
		
		DrawModelAndDepth(rc);
		EffectEngine::GetInstance()->Draw();
		m_postEffect->Render(rc);

		Render2DDraw(rc);
		m_renderobject.clear();
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
	}
}