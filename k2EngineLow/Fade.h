#pragma once

namespace nsK2EngineLow {
	enum EnFadeImage
	{
		enFadeImage_Loading,
		enFadeImage_None
	};
	class Fade
	{
	public:


		void SetFadeImage(EnFadeImage image)
		{
			m_fadeImage = image;
		}
		void Init();
		void Render(RenderContext& renderContext);
		void Update();
		/// <summary>
		/// フェードイン。
		/// </summary>
		void StartFadeIn()
		{
			m_state = enState_FadeIn;
		}
		/// <summary>
		/// フェードアウト。
		/// </summary>
		void StartFadeOut()
		{
			m_state = enState_FadeOut;
		}
		/// <summary>
		/// フェード中＞？
		/// </summary>
		/// <returns>フェード中ならtrue。</returns>
		const bool IsFade() const
		{
			return m_state != enState_Idle;
		}
		/// <summary>
		/// α値を取得。
		/// </summary>
		/// <returns>α値。</returns>
		const float GetCurrentAlpha() const
		{
			return m_currentAlpha;
		}
	private:
		enum EnState {
			enState_FadeIn,			//フェードイン中。
			enState_FadeOut,		//フェードアウト中。
			enState_Idle,			//アイドル中。
		};
		Sprite		m_sprite;				//スプライトレンダ―。
		Sprite		m_sprite2;				//スプライトレンダ―。
		EnState				m_state = enState_Idle;		//状態。
		float				m_currentAlpha = 1.0f;		//現在のα値。ここを変更すると画像が透明になる。
		EnFadeImage m_fadeImage = enFadeImage_Loading;
	};
	extern Fade g_fade;
}
