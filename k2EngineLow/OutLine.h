#pragma once
namespace nsK2EngineLow
{
	class OutLine
	{
	public:
		//void Init();
		//
		//void Draw(RenderContext& rc);

	//	RenderTarget* rts[] = {
	//&g_renderingEngine.GetnormalRenderTarget(),
	//&g_renderingEngine.GetdepthOutLineRenderTarget()
	//	};
	//	//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂�
	//	rc.WaitUntilToPossibleSetRenderTargets(2, rts);
	//	//�����_�����O�^�[�Q�b�g��ݒ�B
	//	rc.SetRenderTargetsAndViewport(2, rts);
	//	// �����_�����O�^�[�Q�b�g���N���A
	//	rc.ClearRenderTargetViews(2, rts);
	//	for (auto& renderObj : m_renderobject) {
	//		renderObj->OnRenderOutLineModel(rc);
	//	}
	//	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	//	rc.WaitUntilFinishDrawingToRenderTargets(2, rts);
	private:
		Sprite m_sprite;

	};
}

