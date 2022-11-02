#pragma once
class Slow;
class Cannon :public IGameObject
{
public:
	Cannon() {};
	~Cannon();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(Quaternion rot)
	{
		m_rotation = rot;
	}
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
private:
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	ModelRender m_modelRender;
	float m_timer = 0.0f;
	float m_NoDamageTimer = 0.0f;
	bool m_NoDamage = false;
	Slow* m_slow;
	CollisionObject* m_CO;
	PhysicsStaticObject m_PSO;
	CharacterController m_charaCon;
	int m_HP = 5.0f;
};

