#pragma once
class Stage:public IGameObject
{
public:
	~Stage();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(Vector3& pos)
	{
		m_position = pos;
	}
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}
private:
	ModelRender m_backGround_Plane;
	ModelRender m_backGround_Building;
	ModelRender m_backGround_Collision;
	PhysicsStaticObject m_PSO;
	Vector3 m_position;
	Vector3 m_scale=Vector3::One;
	Quaternion m_rotation;
};

