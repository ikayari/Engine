#pragma once
class Player;
#include "Level2DRender.h"
class PlayerUI :public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetMaxHP(float& mHP)
	{
		m_maxHP = mHP;
	}
	void SetNowHP(float& nHP)
	{
		m_nowHP = nHP;
	}
	void DisableUI()
	{
		m_draw = false;
	}
	void EnableUI()
	{
		m_draw = true;
	}
private:
	SpriteRender m_gauge;
	SpriteRender m_bar;
	float m_pivotDiff;
	float m_maxHP;
	float m_nowHP;
	float m_percent;
	Vector3 m_scale;
	Vector3 m_position;
	Level2DRender m_playerHP;
	Player* m_player;

	bool m_draw = true;
};

