#pragma once
#include "Level2DRender.h"
class Player;
class EnemyUI :public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetEnemyPosition(Vector3& pos)
	{
		m_enemyPosition = pos;
	}
	void SetPlayer(Player* pl)
	{
		m_player = pl;
	}
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
	Vector3 m_enemyPosition;
	Level2DRender m_enemyHP;
	Player* m_player;

	bool m_isDraw = false;
	bool m_draw = true;

};

