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
		float m_clip=0.0f;
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
			m_modelRenderCB.m_clip = n;
		}
		const float GetRed()const
		{
			return m_modelRenderCB.m_clip;
		}

		void ShadowMapDraw(RenderContext& rc);

		void Execute(RenderContext& rc);

		void DrawModelAndDepth(RenderContext& rc);

		void DrawOutLine(RenderContext& rc);

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
		RenderTarget& GetdepthOutLineRenderTarget()
		{
			return m_depthOutLineRenderTarget;
		}
		RenderTarget& GetnormalRenderTarget()
		{
			return m_normalRenderTarget;
		}
	private:
		std::vector<IRenderer*> m_renderobject;
		ShadowMapRender m_shadowMapRender;
		PostEffect* m_postEffect;
		ModelRenderCB m_modelRenderCB;
		RenderTarget m_mainRenderTarget;
		RenderTarget m_depthRenderTarget;
		RenderTarget m_depthOutLineRenderTarget;
		RenderTarget m_velocityRenderTarget;
		RenderTarget m_normalRenderTarget;
	};
	
	extern RenderingEngine g_renderingEngine;
}
