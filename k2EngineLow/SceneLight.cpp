#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"


namespace nsK2EngineLow {
	SceneLight g_sceneLight;

	SceneLight::SceneLight()
	{
		Vector3 dir = { 1.0f,-0.5f,-1.0f };
		m_light.directionLight.SetDirection(dir);

		m_light.directionLight.SetColor({ 1.0f,1.0f,1.0f });

		SetAmbientLight({0.3f,0.3f,0.3f});

		m_light.pointLight.SetPosition({ 0.0f,50.0f,50.0 });

		m_light.pointLight.SetColor({ 15.0f,0.0f,0.0f });

		m_light.pointLight.SetRange(0.0f);


		m_light.spotLight.SetPosition({ 0.0f,50.0f,100.0f });

		m_light.spotLight.SetColor({0.0f,0.0f,0.0f });
		dir = { 1.0f,-1.0f,1.0f };
		m_light.spotLight.SetDirection(dir);

		m_light.spotLight.SetRange(300.0f);

		m_light.spotLight.SetAngle(Math::DegToRad(25.0f));
	}









};