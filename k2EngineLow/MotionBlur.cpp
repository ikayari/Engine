#include "k2EngineLowPreCompile.h"
#include "MotionBlur.h"
namespace nsK2EngineLow
{
	void MotionBlur::InitBlurSprite()
	{
		
		//初期化情報を作成する。
		SpriteInitData motionBlurSpriteData;
		//ブラー用のシェーダー。
		motionBlurSpriteData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
		//頂点シェーダーのエントリーポイントを指定する。
		motionBlurSpriteData.m_vsEntryPointFunc = "VSMain";
		//ピクセルシェーダーのエントリーポイントを指定する。
		motionBlurSpriteData.m_psEntryPoinFunc = "PSMain";
		//スプライトの幅と高さはluminnceRenderTargetと同じ。
		motionBlurSpriteData.m_width = 1600;
		motionBlurSpriteData.m_height = 900;
		//テクスチャはメインレンダリングターゲットのカラーバッファ。
		motionBlurSpriteData.m_textures[0] = &g_renderingEngine.GetmainRenderTarget().GetRenderTargetTexture();
		motionBlurSpriteData.m_textures[1] = &g_renderingEngine.GetvelocityRenderTarget().GetRenderTargetTexture();
		//描き込むレンダリングターゲットのフォーマットを指定する。
		motionBlurSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		motionBlurSpriteData.m_alphaBlendMode = AlphaBlendMode_None;

		//作成した初期化情報をもとにスプライトを初期化する。

		m_motionBlurSprite.Init(motionBlurSpriteData);


	}
	void MotionBlur::Render(RenderContext& rc)
	{
		m_motionBlurSprite.Draw(rc);
	}
}