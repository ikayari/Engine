#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// 連番アニメーション。
	/// 連番にしたい画像は「unityChan_0.dds」や、「unityChan_10.dds」のような名前にしてください。
	/// </summary>
	class SerialNumberSpriteRender
	{
	public:
		enum EnState
		{
			enStop,
			enPlay
		};
		/// <summary>
		/// 連番アニメーションの読み込み
		/// </summary>
		/// <param name="filePath">ファイルパス。_より前。unityChan_0.ddsを使うときはunityChanまで。</param>
		/// <param name="w">横の長さ</param>
		/// <param name="h">縦の長さ</param>
		/// <param name="n">連番アニメーションの数。</param>
		/// <param name="alphaBlendMode">ブレンディングモード。</param>
		void Init(const char* filePath, const float w, const float h, const int n, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
		/// <summary>
		/// 画像の更新　画像の再生処理も兼ねているので再生したければ毎フレーム呼び出すべきです。（大本営発表）
		/// </summary>
		void Update();
		/// <summary>
		/// 再生開始。
		/// </summary>
		void Play()
		{
			m_state = enPlay;
		}
		/// <summary>
		/// 再生停止。
		/// </summary>
		void Stop()
		{
			m_state = enStop;
		}
		/// <summary>
		/// 乗算カラーを設定。
		/// </summary>
		/// <param name="mulColor">乗算カラー。</param>
		void SetMulColor(const Vector4& mulColor)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetMulColor(mulColor);
			}
		}

		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot">ピボット。</param>
		void SetPivot(const Vector2& pivot)
		{
			if (!m_isInit)
			{
				return;
			}
			m_pivot = pivot;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot">回転。</param>
		void SetRotation(const Quaternion& rot)
		{
			if (!m_isInit)
			{
				return;
			}
			m_rotation = rot;
		}
		/// <summary>
		/// 座標を設定。zは0.0fで。
		/// </summary>
		/// <param name="pos">座標。</param>
		void SetPosition(const Vector3& pos)
		{
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetPosition(pos);
			}
		}
	
		/// <summary>
		/// 大きさを設定。zは1.0fで。
		/// </summary>
		/// <param name="scale">大きさ。</param>
		void SetScale(const Vector3& scale)
		{
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetScale(scale);
			}
		}
	

		/// <summary>
		/// 画像を描画する割合。
		/// </summary>
		/// <param name="x">描画する割合。1.0fが最大。</param>
		void SetLimitedX(const float x)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetLimitedX(x);
			}
			
		}
		void SetLimitedY(const float y)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetLimitedY(y);
			}
		}

		/// <summary>
		/// 原点を設定。原点を元として引数の分だけ画像を生成する。
		/// Left 0.5だと左から五割だけ画像が描写される。
		/// </summary>
		/// <param name="isLeft">原点。</param>
		void SetIsDisplayRestrictionLeftSide(const bool isLeft)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetIsDisplayRestrictionLeftSide(isLeft);
			}
		}
		void SetIsDisplayRestrictionRightSide(const bool isRight)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetIsDisplayRestrictionRightSide(isRight);
			}
		}

		void SetIsDisplayRestrictionUpSide(const bool isUp)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetIsDisplayRestrictionUpSide(isUp);
			}
		}
		void SetIsDisplayRestrictionDownSide(const bool isDown)
		{
			if (!m_isInit)
			{
				return;
			}
			int spritenumber = m_spriteRenders.size();
			for (int i = 0; i < spritenumber; i++)
			{
				m_spriteRenders[i]->SetIsDisplayRestrictionDownSide(isDown);
			}
		}
		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// 描画画像の順番をセット。
		/// </summary>
		/// <param name="n">描画したい画像の順番。</param>
		void SetDrawNumber(float n)
		{
			m_drawnumber = n;
		}
		/// <summary>
		/// 描画速度。
		/// </summary>
		/// <param name="speed">指定した速度倍に再生されます。通常は1/4フレームに一回更新だと思います。</param>
		void SetDrawSpeed(int speed)
		{
			m_drawspeed = speed;
		}
		/// <summary>
		/// 描画されてる画像順を取得します。
		/// </summary>
		/// <returns>現在の描画画像順</returns>
		const float& GetDrawNumber()const
		{
			return m_drawnumber;
		}
		/// <summary>
		/// スプライトレンダーのサイズを取得します。
		/// </summary>
		/// <returns>スプライトレンダーのサイズ</returns>
		const int& GetSpriteRendersSize()const
		{
			return m_Maxnumber;
		}
		/// <summary>
		/// スプライトレンダーを破棄します。
		/// </summary>
		void Release()
		{
			m_isInit = false;
			m_spriteRenders.clear();
		}
		/// <summary>
		/// 描画順を0に戻します
		/// </summary>
		void DrawNumberReset()
		{
			m_drawnumber = 0;
		}
		/// <summary>
		/// ループフラグ
		/// </summary>
		/// <param name="loop">trueにするとループ。</param>
		void SetLoopFlag(bool loop)
		{
			m_isLoop=loop;
		}
	private:
		std::vector<std::unique_ptr<SpriteRender>> m_spriteRenders;
		Vector3			m_position = Vector3::Zero;				//座標。
		Quaternion		m_rotation = Quaternion::Identity;		//回転。
		Vector3			m_scale = Vector3::One;					//大きさ。
		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット。
		bool m_isInit = false;
		EnState m_state = enStop;
		int m_Maxnumber=0;
		float m_drawnumber=0;
		float m_drawratio = 0.25f;
		float m_drawspeed = 1.0f;
		bool m_isLoop = false;
		void SetState(EnState rooptype)
		{
			m_state = rooptype;
		}
		const EnState GetState()const
		{
			return m_state;
		}
	};

};