#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	PostEffect g_postEffect;

	void PostEffect::Init()
	{

		m_motionBlur.InitBlurSprite();
		//解像度、ミップマップレベル
		luminanceRenderTarget.Create(
			1600,		//解像度はメインレンダリングターゲットと同じ。
			900,		//解像度はメインレンダリングターゲットと同じ。
			1,
			1,
			//【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

	}

	void PostEffect::Render(RenderContext& rc)
	{
		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(g_postEffect.luminanceRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTarget(g_postEffect.luminanceRenderTarget);
		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(g_postEffect.luminanceRenderTarget);

		g_bloom.LuminanceSpriteDraw(rc);

		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(g_postEffect.luminanceRenderTarget);

		g_bloom.Blur(rc);

		g_bloom.Render(rc, g_renderingEngine.GetmainRenderTarget());
		// step-5 画面に表示されるレンダリングターゲットに戻す
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		g_bloom.Draw(rc);
		m_motionBlur.Render(rc);
		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &g_renderingEngine.GetvelocityRenderTarget().GetRenderTargetTexture();
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		Sprite spsp;
		spsp.Init(spriteInitData);
		if (g_pad[0]->IsPress(enButtonSelect))
		{
			spsp.Draw(rc);
		}
	}
	void PostEffect::MotionBlurDraw(RenderContext& rc)
	{
		m_motionBlur.Render(rc);
	}
}