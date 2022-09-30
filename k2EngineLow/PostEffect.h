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
	private:
		MotionBlur m_motionBlur;
		Bloom m_bloom;
	};

	extern PostEffect g_postEffect;

}
