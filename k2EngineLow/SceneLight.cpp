#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"


namespace nsK2EngineLow {
	SceneLight g_sceneLight;

	SceneLight::SceneLight()
	{
		Vector3 dir = { 1.0f,-0.5f,1.0f };
		m_light.directionLight[0].SetDirection(dir);
		m_light.directionLight[0].SetColor(Vector3::One);
		dir = { 1.0f,-0.5f,-1.0f };
		m_light.directionLight[1].SetDirection(dir);
		m_light.directionLight[1].SetColor(Vector3::One);
		dir = { -1.0f,-0.5f,1.0f };
		m_light.directionLight[2].SetDirection(dir);
		m_light.directionLight[2].SetColor(Vector3::One);
		dir = { -1.0f,-0.5f,-1.0f };
		m_light.directionLight[3].SetDirection(dir);
		m_light.directionLight[3].SetColor(Vector3::One);

		SetAmbientLight({0.4f,0.4f,0.4f});
		m_light.pointLight.SetPosition({ 0.0f,50.0f,50.0 });

		m_light.pointLight.SetColor({ 0.0f,0.0f,0.0f });

		m_light.pointLight.SetRange(0.0f);


		m_light.spotLight.SetPosition({ 0.0f,50.0f,100.0f });

		m_light.spotLight.SetColor({0.0f,0.0f,0.0f });
		dir = { 1.0f,-1.0f,1.0f };
		m_light.spotLight.SetDirection(dir);

		m_light.spotLight.SetRange(0.0f);

		m_light.spotLight.SetAngle(Math::DegToRad(0.0f));
	}









};