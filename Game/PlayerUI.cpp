#include "stdafx.h"
#include "PlayerUI.h"

bool PlayerUI::Start()
{
	m_playerHP.Init("Assets/sprite/UI/Player/PlayerHP.casl", [&](Level2DObjectData& objData) {
		if (objData.EqualObjectName("Gauge") == true) {

			//レベルのデータを使用して画像を読み込む。
			m_gauge.Init(objData.ddsFilePath, objData.width, objData.height);
			m_gauge.SetScale(objData.scale);
			m_gauge.SetPosition(objData.position);
			m_gauge.SetPivot(objData.pivot);


			return true;
		}
		else if (objData.EqualObjectName("Bar") == true) {

			//レベルのデータを使用して画像を読み込む。
			m_bar.Init(objData.ddsFilePath, objData.width, objData.height);
			m_bar.SetScale(objData.scale);
			m_scale = objData.scale;
			m_bar.SetPosition(objData.position);
			m_bar.SetPivot(objData.pivot);

			return true;
		}
	return false;
		});
	//m_bar.SetPosition(m_position);
	//m_gauge.SetPosition(m_position);
	/*m_bar.SetMulColor({ 1.0f,1.0f,1.0f,0.75f });
	m_gauge.SetMulColor({ 1.0f,1.0f,1.0f,0.75f });*/
	m_bar.Update();
	m_gauge.Update();
	return true;
}

void PlayerUI::Update()
{
	m_percent = m_nowHP / m_maxHP;
	Vector3 barScale = m_scale;
	barScale.x *= m_percent;
	m_bar.SetScale(barScale);
	m_bar.Update();
	m_gauge.Update();
	
}

void PlayerUI::Render(RenderContext& rc)
{
	if (m_draw)
	{
		m_bar.Draw(rc);
		m_gauge.Draw(rc);
	}
}
