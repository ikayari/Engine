#pragma once
#include "graphics/GaussianBlur.h"
namespace nsK2EngineLow {
	class Bloom
	{
	public:

		void InitLuminanceSprite();
		void InitFinalSprite();
		void InitSprite();
		void InitBlur();

		void Init();

		void Blur(RenderContext& rc);

		void Render(RenderContext& rc,RenderTarget& rt);

		void Draw(RenderContext& rc);

		void LuminanceSpriteDraw(RenderContext& rc)
		{
			luminanceSprite.Draw(rc);
		}

		GaussianBlur gaussianBlur[4];
		Sprite finalSprite;
		Sprite luminanceSprite;
		Sprite copyToFrameBufferSprite;
	};


}
