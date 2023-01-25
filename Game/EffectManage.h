#pragma once
class EffectManage : public IGameObject
{
public:
	bool Start();

	void PlayEffect(int n,Vector3 pos, Vector3 scale=Vector3::One, Quaternion rot = Quaternion::Identity);


};

