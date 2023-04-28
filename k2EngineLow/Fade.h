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
		/// �t�F�[�h�C���B
		/// </summary>
		void StartFadeIn()
		{
			m_state = enState_FadeIn;
		}
		/// <summary>
		/// �t�F�[�h�A�E�g�B
		/// </summary>
		void StartFadeOut()
		{
			m_state = enState_FadeOut;
		}
		/// <summary>
		/// �t�F�[�h�����H
		/// </summary>
		/// <returns>�t�F�[�h���Ȃ�true�B</returns>
		const bool IsFade() const
		{
			return m_state != enState_Idle;
		}
		/// <summary>
		/// ���l���擾�B
		/// </summary>
		/// <returns>���l�B</returns>
		const float GetCurrentAlpha() const
		{
			return m_currentAlpha;
		}
	private:
		enum EnState {
			enState_FadeIn,			//�t�F�[�h�C�����B
			enState_FadeOut,		//�t�F�[�h�A�E�g���B
			enState_Idle,			//�A�C�h�����B
		};
		Sprite		m_sprite;				//�X�v���C�g�����_�\�B
		Sprite		m_sprite2;				//�X�v���C�g�����_�\�B
		EnState				m_state = enState_Idle;		//��ԁB
		float				m_currentAlpha = 1.0f;		//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
		EnFadeImage m_fadeImage = enFadeImage_Loading;
	};
	extern Fade g_fade;
}
