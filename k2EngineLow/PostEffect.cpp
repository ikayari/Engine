#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	PostEffect g_postEffect;

	void PostEffect::Init()
	{

		m_motionBlur.InitBlurSprite();
		//�𑜓x�A�~�b�v�}�b�v���x��
		luminanceRenderTarget.Create(
			1600,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
			900,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
			1,
			1,
			//�y���ځz�J���[�o�b�t�@�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���B
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
		m_bloom.Init();

	}

	void PostEffect::Render(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(g_postEffect.luminanceRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTarget(g_postEffect.luminanceRenderTarget);
		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(g_postEffect.luminanceRenderTarget);

		m_bloom.LuminanceSpriteDraw(rc);

		// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(g_postEffect.luminanceRenderTarget);

		m_bloom.Blur(rc);

		m_bloom.Render(rc, g_renderingEngine.GetmainRenderTarget());
		// step-5 ��ʂɕ\������郌���_�����O�^�[�Q�b�g�ɖ߂�
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_bloom.Draw(rc);
		m_motionBlur.Render(rc);
		SpriteInitData luminanceSpriteInitData;
		
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/OutLine_PostEffect.fx";
		
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSMain";
		
		luminanceSpriteInitData.m_width = 1600;
		luminanceSpriteInitData.m_height = 900;
		
		luminanceSpriteInitData.m_textures[0] = &g_renderingEngine.GetnormalRenderTarget().GetRenderTargetTexture();
		luminanceSpriteInitData.m_textures[1] = &g_renderingEngine.GetdepthOutLineRenderTarget().GetRenderTargetTexture();
		//�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//�쐬�����������������ƂɃX�v���C�g������������B
		luminanceSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Multiply;
		Sprite sprite;
		sprite.Init(luminanceSpriteInitData);
		sprite.Draw(rc);
	}
	void PostEffect::MotionBlurDraw(RenderContext& rc)
	{
		m_motionBlur.Render(rc);
	}
}