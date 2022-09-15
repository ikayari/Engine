#pragma once
#include "SceneLight.h"
#include "ShadowMapRender.h"
#include "PostEffect.h"

namespace nsK2EngineLow
{
	struct ModelRenderCB
	{
	public:
		Light m_light;          // ライト
		Matrix m_lvp; // ライトビュープロジェクション行列。
		Vector3 m_playerPosition = { 0.0f,0.0f,0.0f };
		float m_red;
		Vector3 pad;
		Matrix m_prevWorldMatrix;
		Matrix m_prevViewMatrix;
		Matrix m_prevProjectionMatrix;
		

	};
	class RenderingEngine :public Noncopyable
	{
	public:
		void Init();
		void AddRenderObject(IRenderer* renderobj)
		{
			m_renderobject.push_back(renderobj);
		}

		void Render2DDraw(RenderContext& rc);


		void SetPlayerPosition(Vector3 pos)
		{
			m_modelRenderCB.m_playerPosition = pos;
		}

		void SetRed(float n)
		{
			m_modelRenderCB.m_red = n;
		}
		const float GetRed()const
		{
			return m_modelRenderCB.m_red;
		}

		void ShadowMapDraw(RenderContext& rc);

		void Execute(RenderContext& rc);

		void DrawModelAndDepth(RenderContext& rc);

		Texture& GetShadowMap()
		{
			return m_shadowMapRender.GetShadowMap();
		}
		Camera& GetLightCamera()
		{
			return m_shadowMapRender.GetLightCamera();
		}
		ModelRenderCB& GetModelRenderCB()
		{
			return m_modelRenderCB;
		}
		RenderTarget& GetDepthRenderTarget()
		{
			return m_depthRenderTarget;
		}
		RenderTarget& GetmainRenderTarget()
		{
			return m_mainRenderTarget;
		}
		RenderTarget& GetvelocityRenderTarget()
		{
			return m_velocityRenderTarget;
		}
	private:
		std::vector<IRenderer*> m_renderobject;
		ShadowMapRender m_shadowMapRender;
		PostEffect* m_postEffect = &g_postEffect;
		ModelRenderCB m_modelRenderCB;
		RenderTarget m_mainRenderTarget;
		RenderTarget m_depthRenderTarget;
		RenderTarget m_velocityRenderTarget;
	};
	
	extern RenderingEngine g_renderingEngine;
}
