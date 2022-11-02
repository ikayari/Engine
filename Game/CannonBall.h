#pragma once
class Slow;
class CannonBall:public IGameObject
{
public:
	CannonBall() {};
	~CannonBall() ;
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}
	void SetScaleRatio(float scale)
	{
		m_scaleRatio = scale;
	}
private:
	float m_timer = 0.0f;
	Vector3 m_position;
	Vector3 m_forward;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation;
	ModelRender m_modelRender;
	CollisionObject* m_CO;
	Slow* m_slow;
	float m_scaleRatio = 1.0f;
};

