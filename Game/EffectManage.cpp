#include "stdafx.h"
#include "EffectManage.h"
#include "graphics/effect/EffectEmitter.h"

bool EffectManage::Start()
{
	//�G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efk/ball2.efk");
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/efk/hit.efk");

	return true;
}
void EffectManage::PlayEffect(int n, Vector3 pos, Vector3 scale, Quaternion rot)
{
	//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(n);
	//�G�t�F�N�g�̍��W��ݒ肷��B
	effectEmitter->SetPosition(pos);
	//�G�t�F�N�g�̉�]��ݒ肷��
	effectEmitter->SetRotation(rot);
	//�G�t�F�N�g�̑傫�����w�肷��B
	effectEmitter->SetScale(scale);


	effectEmitter->Play();
}