#include "stdafx.h"
#include "cEffectManager.h"
#include "cEffect.h"
#include "cEffectParticle.h"
#include "cEffectMesh.h"

cEffectManager::cEffectManager()
{
}


cEffectManager::~cEffectManager()
{
	for (auto p : m_setEffects){
		SAFE_RELEASE(p);
	}

	while (!m_qeueuOnHitEffectPool.empty()){
		cEffect* p = m_qeueuOnHitEffectPool.front();
		m_qeueuOnHitEffectPool.pop();
		SAFE_RELEASE(p);
	}

	while (!m_qeueuOnGetHitEffectPool.empty()){
		cEffect* p = m_qeueuOnGetHitEffectPool.front();
		m_qeueuOnGetHitEffectPool.pop();
		SAFE_RELEASE(p);
	}
}

void cEffectManager::Setup(){
	// Pre-Load
	for (UINT i = 0; i < 10; i++){
		cEffectParticle* p = new cEffectParticle;
		m_qeueuOnHitEffectPool.push(p);
		p->SetOwner(this);
	}
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

	if (!m_vecOnHitEffectTobeDeleted.empty()){
		for (auto p : m_vecOnHitEffectTobeDeleted){
			m_setEffects.erase(p);
			if (p->GetEffectType() == p->E_EFFECT_ONHIT){
				m_qeueuOnGetHitEffectPool.push(p);
			}
		}
		m_vecOnHitEffectTobeDeleted.clear();
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

void cEffectManager::AddEffect(UINT uiType, D3DXVECTOR3 vPos){
	if (uiType == cEffect::E_EFFECT_ONHIT){
		if (m_qeueuOnHitEffectPool.empty()){
			cEffectParticle* p = new cEffectParticle;
			p->Setup();
			p->SetPosition(vPos);
			p->SetOwner(this);
			m_setEffects.insert(p);
		}
		else{
			cEffect* p = m_qeueuOnHitEffectPool.front();
			m_qeueuOnHitEffectPool.pop();
			p->Setup();
			p->SetPosition(vPos);
			m_setEffects.insert(p);
		}
	}
}

void cEffectManager::DeleteEffect(cEffect* pEffect){
	if (pEffect){
		m_vecOnHitEffectTobeDeleted.push_back(pEffect);
	}
}

void cEffectManager::Destroy(){
	// vector to be deleted 는 어차피 삭제될 놈들이므로 신경 안써도 댐

	this->Release();
}