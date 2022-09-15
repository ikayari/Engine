#pragma once
namespace nsK2EngineLow {
	class DepthOfField
	{
	public:
		void Init();

		void Render(RenderContext& rc);
	};
	extern DepthOfField g_depthOfField;
}