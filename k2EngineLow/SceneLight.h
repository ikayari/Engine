#pragma once

namespace nsK2EngineLow {

	//ディレクションライト。
	struct DirectionLight {
	private:
		Vector3 Direction;//ライトの⽅向。wは影を落とすかの処理。
		float pad;
		//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、C++側にはパディングを埋めておく。
		Vector3 Color;//ライトのカラー。
		float pad1;

	public:
		void SetDirection(Vector3& dir)
		{
			dir.Normalize();
			Direction = dir;
		}
		void SetColor(const Vector3& col)
		{
			Color = col;
		}
		const Vector3& GetDirection()const
		{
			return Direction;
		}
	};

	struct PointLight
	{
	private:
		Vector3 Position;
		float pad;
		Vector3 Color;
		float Range;
	public:
		void SetPosition(const Vector3& pos)
		{
			Position = pos;
		}
		void SetColor(const Vector3& col)
		{
			Color = col;
		}
		void SetRange(const float& rg)
		{
			Range = rg;
		}
		const Vector3 GetPosition()const 
		{
			return Position;
		}
	};
	struct SpotLight
	{
	private:
		Vector3 Position;			//位置
		float pad;					//パディング
		Vector3 Color;				//カラー
		float Range;				//影響範囲
		Vector3 Direction;			//射出方向
		float Angle;				//射出角度
	public:
		void SetPosition(const Vector3& pos)
		{
			Position = pos;
		}
		void SetColor(const Vector3& col)
		{
			Color = col;
		}
		void SetRange(const float& rg)
		{
			Range = rg;
		}
		void SetDirection(Vector3& dir)
		{
			dir.Normalize();
			Direction = dir;
		}
		void SetAngle(const float& ag)
		{
			Angle = ag;
		}

		void ApplyRotation(const Quaternion rot)
		{
			rot.Apply(Direction);
		}

		const Vector3& GetDirection()const
		{
			return Direction;
		}
		
	};

	struct Light
	{
		DirectionLight directionLight;
		PointLight pointLight;
		SpotLight spotLight;
		Vector3 ambientLight;
		float pad;
		Vector3 eyePos;
		float pad2;
	};

		class SceneLight
		{



		public:
			SceneLight();
			/// <summary>
			/// ライト構造体を取得する。
			/// </summary>
			/// <returns>ライト。</returns>
			Light& GetLight()
			{
				return m_light;
			}
			/// <summary>
			/// 環境光を設定する。
			/// </summary>
			/// <param name="col">カラー。</param>
			void SetAmbientLight(const Vector3& col)
			{
				m_light.ambientLight = col;
			}
			/// <summary>
			/// 視点のポジションを設定する。
			/// </summary>
			/// <param name="pos">カメラのポジション。</param>
			void SeteyePosition(const Vector3& pos)
			{
				m_light.eyePos = pos;
			}
			/// <summary>
			/// ポイントライトのポジションを設定する。
			/// </summary>
			/// <param name="pos">座標。</param>
			void SetPointLightPosition(const Vector3& pos)
			{
				m_light.pointLight.SetPosition(pos);
			}
			/// <summary>
			/// ポイントライトのポジションを取得する。
			/// </summary>
			/// <returns>ポイントライトの座標。</returns>
			const Vector3 GetPointLightPosition()const 
			{
				return m_light.pointLight.GetPosition();
			}
			const Vector4& GetDirectionLightDirection()const
			{
				return m_light.directionLight.GetDirection();
			}
			/// <summary>
			/// スポットライトに回転を加算する。
			/// </summary>
			/// <param name="rot">ローテーション。</param>
			void ApplySpotLightRotation(const Quaternion& rot)
			{
				m_light.spotLight.ApplyRotation(rot);
			}
			/// <summary>
			/// スポットライトの方向を取得する。
			/// </summary>
			/// <returns>方向。</returns>
			const Vector3& GetSpotLightDirection()const
			{
				return m_light.spotLight.GetDirection();
			}
			/// <summary>
			/// ディレクションライトの色を設定する。
			/// </summary>
			/// <param name="col">カラー。</param>
			void SetDirectionColor(const Vector3& col)
			{
				m_light.directionLight.SetColor(col);
			}


		private:
			Light m_light;
		};
		extern SceneLight g_sceneLight;
	}