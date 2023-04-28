#include "k2EngineLowPreCompile.h"
#include "Fade.h"
namespace nsK2EngineLow {
	Fade g_fade;
	void Fade::Init()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_ddsFilePath[0] = "Assets/sprite/Fade/fade.dds";
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		m_sprite.Init(spriteInitData);
		SpriteInitData spriteInitData2;
		spriteInitData2.m_ddsFilePath[0] = "Assets/sprite/Fade/None.dds";
		spriteInitData2.m_width = 1600;
		spriteInitData2.m_height = 900;
		spriteInitData2.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData2.m_alphaBlendMode = AlphaBlendMode_Trans;
		m_sprite2.Init(spriteInitData2);
		
	}

	void Fade::Render(RenderContext& renderContext)
	{
		if (m_currentAlpha > 0.0f) {
			switch (m_fadeImage)
			{
			case enFadeImage_Loading:
				m_sprite.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
				m_sprite.Draw(renderContext);
				break;
			case enFadeImage_None:
				m_sprite2.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
				m_sprite2.Draw(renderContext);
				break;
			default:
				break;
			}

			
		}
	}
	void Fade::Update()
	{
		switch (m_state) {
		case enState_FadeIn:
			m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
			if (m_currentAlpha <= 0.0f) {
				m_currentAlpha = 0.0f;
				m_state = enState_Idle;
			}
			break;
		case enState_FadeOut:
			m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
			if (m_currentAlpha >= 1.0f) {
				m_currentAlpha = 1.0f;
				m_state = enState_Idle;
			}
			break;
		case enState_Idle:

			break;
		}
	}
}