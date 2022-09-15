#pragma once
namespace nsK2EngineLow {

	class MotionBlur
	{
	public:
		void InitBlurSprite();
		void Render(RenderContext& rc);
	private:
		Sprite m_motionBlurSprite;
	};

}