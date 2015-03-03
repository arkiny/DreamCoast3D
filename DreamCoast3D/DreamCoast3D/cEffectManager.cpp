#include "stdafx.h"
#include "cEffectManager.h"
#include "cEffect.h"
#include "cEffectFireBall.h"


cEffectManager::cEffectManager()
{
}


cEffectManager::~cEffectManager()
{
	for (auto p : m_setEffects){
		SAFE_RELEASE(p);
	}
}

void cEffectManager::Setup(){

}

void cEffectManager::Start(){
	for (auto p : m_setEffects){
		p->SetOwner(this);
	}
}

void cEffectManager::Update(float fDelta){
	for (auto p : m_setEffects){
		p->Update(fDelta);
	}

	if (!m_vecEffectTobeDeleted.empty()){
		for (auto p : m_vecEffectTobeDeleted){
			m_setEffects.erase(p);
			SAFE_RELEASE(p);
		}
		m_vecEffectTobeDeleted.clear();
	}
}

void cEffectManager::Render(){
	for (auto p : m_setEffects){
		p->Render();
	}
}

void cEffectManager::Exit(){
	// 일단 대기
}

void cEffectManager::AddEffect(cEffect* pEffect){
	if (pEffect){
		SAFE_ADD_REF(pEffect);
		pEffect->SetOwner(this);
		m_setEffects.insert(pEffect);
	}
}

void cEffectManager::DeleteEffect(cEffect* pEffect){
	if (pEffect){
		m_vecEffectTobeDeleted.push_back(pEffect);
	}
}

void cEffectManager::Destroy(){
	// vector to be deleted 는 어차피 삭제될 놈들이므로 신경 안써도 댐

	this->Release();
}