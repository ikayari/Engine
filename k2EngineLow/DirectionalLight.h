/*#pragma once
namespace nsK2EngineLow {




		struct DirectionLight {
		private:
			Vector3 ligDirection; //ライトの⽅向。
			//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、C++側にはパディングを埋めておく。
			float pad;
			Vector3 ligColor; //ライトのカラー。
			Vector3 eyePos; //視点の位置。
		public:
			DirectionLight& GetDirectionLight()
			{
				return m_directionLight;
			}
			void SetColor(const Vector3& color)
			{
				m_ligColor = color;
				m_directionLight.ligColor = m_ligColor;
			}
			void SetDirection(const Vector3& dir)
			{
				m_ligDirection = dir;
				m_ligDirection.Normalize();
				m_directionLight.ligDirection = m_ligDirection;
			}
			void SeteyePosition(const Vector3& pos)
			{
				m_eyePos = pos;
				m_directionLight.eyePos = m_eyePos;
			}
		private:


			Vector3 m_eyePos;

			Vector3 m_ligDirection;
			Vector3 m_ligColor;
			DirectionLight m_directionLight;
		};

		

		
	extern DirectionLight g_directionLight;
}*/