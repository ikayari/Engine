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
	//	//レンダリングターゲットとして利用できるまで待つ
	//	rc.WaitUntilToPossibleSetRenderTargets(2, rts);
	//	//レンダリングターゲットを設定。
	//	rc.SetRenderTargetsAndViewport(2, rts);
	//	// レンダリングターゲットをクリア
	//	rc.ClearRenderTargetViews(2, rts);
	//	for (auto& renderObj : m_renderobject) {
	//		renderObj->OnRenderOutLineModel(rc);
	//	}
	//	// レンダリングターゲットへの書き込み終了待ち
	//	rc.WaitUntilFinishDrawingToRenderTargets(2, rts);
	private:
		Sprite m_sprite;

	};
}

