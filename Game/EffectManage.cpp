#include "stdafx.h"
#include "EffectManage.h"
#include "graphics/effect/EffectEmitter.h"

bool EffectManage::Start()
{
	//エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efk/ball2.efk");
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/efk/hit.efk");

	return true;
}
void EffectManage::PlayEffect(int n, Vector3 pos, Vector3 scale, Quaternion rot)
{
	//エフェクトのオブジェクトを作成する。
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(n);
	//エフェクトの座標を設定する。
	effectEmitter->SetPosition(pos);
	//エフェクトの回転を設定する
	effectEmitter->SetRotation(rot);
	//エフェクトの大きさを指定する。
	effectEmitter->SetScale(scale);


	effectEmitter->Play();
}