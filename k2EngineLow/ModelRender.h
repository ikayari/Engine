#pragma once
#include "RenderingEngine.h"
namespace nsK2EngineLow {
	enum Dithering
	{
		en_normal,
		en_dithering,
		en_pixeldithering
	};
	struct ModelRenderInitData
	{
		Dithering dithering = en_normal;
		bool recieveShadow = false;
		bool NoNormalMap = false;
	};
	class ModelRender : public IRenderer
	{
	public:
		ModelRender();
		~ModelRender();

		void Update();
		/// <summary>
		/// 初期化処理。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void Init(const char* filePath,
			AnimationClip* animationClips=nullptr,
			int numAnimationClips=0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ);
		void SetDithering(Dithering dt)
		{
			m_modelRenderID.dithering = dt;
		}
		void SetRecieveShadow(bool shadow)
		{
			m_modelRenderID.recieveShadow = shadow;
		}
		void SetModelRenderInitData(ModelRenderInitData modelRenderID)
		{
			m_modelRenderID = modelRenderID;
		}
		/// <summary>
		/// 描画処理。
		/// </summary>
		void Draw(RenderContext& rc);
		/// <summary>
		/// 影を落とすかどうか。
		/// </summary>
		/// <param name="castershadow">影を落とす？</param>
		void SetCasterShadow(const bool castershadow)
		{
			m_isShadowCaster = castershadow;
		}
		void SetOutLineDraw(const bool outline)
		{
			m_isOutLineModel = outline;
		}
		/// <summary>
		/// 座標をセット。
		/// </summary>
		/// <param name="pos">座標。</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// 回転をセット。
		/// </summary>
		/// <param name="rot">回転。</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// スケールをセット。
		/// </summary>
		/// <param name="scale">スケール。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(float x, float y, float z)
		{
			SetScale({ x,y,z });
		}
		/// <summary>
		/// 座標、回転、拡大率を設定。
		/// </summary>
		/// <param name="pos">座標。</param>
		/// <param name="rotation">回転。</param>
		/// <param name="scale">拡大率。</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}
		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns></returns>
		Vector3 GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		const bool& IsPlayingAnimation()const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルを取得する。
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		/// <summary>
		/// ディザリング透過度
		/// 1で完全に透過。
		/// </summary>
		/// <param name="clip">クリップ</param>
		void SetClip(const float clip)
		{
			m_clip = clip;
		}
		const float& GetClip()const
		{
			return m_clip;
		}
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		void SetHasNormalMap(bool t)
		{
			m_modelRenderID.NoNormalMap = t;
		}
	private:
		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		/// <param name="animationClip">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
		void InitAnimation(AnimationClip* animationClip,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis);
		/// <summary>
		/// アウトライン描画用モデルの初期化。
		/// </summary>
		/// <param name="tkmFilePath">ファイルパス名</param>
		/// <param name="modelUpAxis">モデルの上方向</param>
		void InitOutLineModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis);
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		void OnRenderShadowMap(
			RenderContext& rc,
			const Matrix& lvpMatrix
		)override;
		/// <summary>
		/// アウトライン描画用モデルを描画する際、描画パスから呼ばれる処理
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void OnRenderOutLineModel(RenderContext& rc)
		{
			if (m_isOutLineModel&&m_outlinemodel.IsInited())
			{
				m_outlinemodelpe.Draw(rc);
			}
		}

		bool IsOutLineModel()const
		{
			return m_isOutLineModel;
		}
		
		Model					m_model;								//モデル。
		Model					m_shadowmodel;							//シャドウモデル。
		Model					m_outlinemodel;							//アウトライン描画用モデル。
		Model					m_outlinemodelpe;
		Animation				m_animation;							//アニメーション。
		AnimationClip*			m_animationClip = nullptr;				//アニメーションクリップ。
		int						m_numAnimationClips = 0;				//アニメーションクリップの数。
		Skeleton				m_skeleton;								//骨。
		float					m_animationSpeed = 1.0f;
		Vector3					m_position = Vector3::Zero;				//座標。	
		Quaternion				m_rotation = Quaternion::Identity;		//回転。
		Vector3					m_scale = Vector3::One;					//拡大率。
		EnModelUpAxis			m_enFbxUpAxis = enModelUpAxisZ;			// FBXの上方向。
		bool					m_isShadowCaster = true;				//シャドウを落とす？
		bool					m_isOutLineModel = false;				//アウトライン描画モデル？
		

		float m_clip = 0.0f;

		ModelRenderCB m_modelCB = g_renderingEngine.GetModelRenderCB();

		ModelRenderInitData m_modelRenderID;

		//bool offScreenRendaring = false;
		
		
		

	};

}
