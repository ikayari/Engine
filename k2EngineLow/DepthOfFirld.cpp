#include "k2EngineLowPreCompile.h"
#include "DepthOfFirld.h"

namespace nsK2EngineLow {

	DepthOfField g_depthOfField;

	void DepthOfField::Init()
	{
		//ガウシアンブラーを作成
		GaussianBlur blur;
		blur.Init(&g_renderingEngine.GetmainRenderTarget().GetRenderTargetTexture());

		//合成用のボケた画像を作成
		SpriteInitData combineBokeSpriteInitData;

		//テクスチャを二枚作成
		combineBokeSpriteInitData.m_textures[0] = &blur.GetBokeTexture();
		combineBokeSpriteInitData.m_textures[1] = &g_renderingEngine.GetDepthRenderTarget().GetRenderTargetTexture();
		combineBokeSpriteInitData.m_width = 1600;
		combineBokeSpriteInitData.m_height = 900;

		//ファイルパスを指定
		combineBokeSpriteInitData.m_fxFilePath = "Assets/shader/Dof.fx";
		combineBokeSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		combineBokeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

		Sprite combineBokeSprite;
		combineBokeSprite.Init(combineBokeSpriteInitData);

		SpriteInitData spriteInitData;

		spriteInitData.m_textures[0] = &g_renderingEngine.GetmainRenderTarget().GetRenderTargetTexture();

		//フレームバッファに合成するので解像度は同じにする
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;

		//通常合成する
		spriteInitData.m_fxFilePath = "Assets/shader/";

		//画像を初期化
		Sprite copyToFrameBufferSprite;
		copyToFrameBufferSprite.Init(spriteInitData);
	}

	void DepthOfField::Render(RenderContext& rc)
	{
	}
}