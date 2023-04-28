#include "stdafx.h"
#include "EnemyUI.h"
#include "Player.h"

const float POSITION_PLUS_Y = 150.0f;

bool EnemyUI::Start()
{
    m_enemyHP.Init("Assets/sprite/UI/Enemy/EnemyHP.casl", [&](Level2DObjectData& objData) {
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
			m_bar.SetPosition(objData.position);
			m_pivotDiff = objData.position.x;
			m_bar.SetPivot(objData.pivot);

			return true;
		}
	return false;
		});
	m_bar.SetMulColor({ 1.0f,1.0f,1.0f,0.75f });
	m_gauge.SetMulColor({ 1.0f,1.0f,1.0f,0.75f });
	m_bar.Update();
	m_gauge.Update();
	
    return true;
}

void EnemyUI::Update()
{
	m_percent = m_nowHP/m_maxHP;
	Vector3 length = g_camera3D->GetPosition() - m_enemyPosition;
	
	float maxDistance = 3000.0f;
	float scale = 1.0f;
	if (length.Length() > maxDistance) {
		scale = 0.0f; // 距離が基準値を超えた場合、スケールを0にする
	}
	else {
		scale = 1.0f - (length.Length() / maxDistance); // 距離に応じてスケールを計算
	};
	m_scale = { scale,scale,1.0f };
	Vector3 barScale = m_scale;
	barScale.x *= m_percent;
	Vector2 pos;
	g_camera3D->CalcScreenPositionFromWorldPosition(pos, m_enemyPosition);
	m_bar.SetScale(barScale);
	m_gauge.SetScale(m_scale);
	m_bar.SetPosition({ pos.x + (m_pivotDiff * scale) ,pos.y + (POSITION_PLUS_Y * scale),0.0f });
	m_gauge.SetPosition({ pos.x,pos.y + (POSITION_PLUS_Y *scale),0.0f });

	m_bar.Update();
	m_gauge.Update();

	
	length.Normalize();
	float Hantei = length.Dot(g_camera3D->GetForward());
	if (Hantei <= 0.5f)
	{
		m_isDraw = true;
	}
	else
	{
		m_isDraw = false;
	}
}

void EnemyUI::Render(RenderContext& rc)
{
	if (m_isDraw&&m_draw)
	{
		m_bar.Draw(rc);
		m_gauge.Draw(rc);
	}
}

