#pragma once
#include "MotionBlur.h"
#include "Bloom.h"
namespace nsK2EngineLow {
	class PostEffect
	{
	public:
		void Init();

		void Render(RenderContext& rc);
		RenderTarget luminanceRenderTarget;
		void MotionBlurDraw(RenderContext& rc);
		RenderTarget& GetdepthOutLineRenderTarget()
		{
			return m_depthOutLineRenderTarget;
		}
		RenderTarget& GetnormalRenderTarget()
		{
			return m_normalRenderTarget;
		}
	private:
		MotionBlur m_motionBlur;
		Bloom m_bloom;
		RenderTarget m_normalRenderTarget;
		RenderTarget m_depthOutLineRenderTarget;
	};

	extern PostEffect g_postEffect;

}
