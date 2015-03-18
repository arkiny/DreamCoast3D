#include "stdafx.h"
#include "cEffectManager.h"
#include "cEffect.h"
#include "cEffectParticle.h"
#include "cEffectPotion.h"
#include "cEffectMesh.h"
#include "cEffectSkill1.h"

cEffectManager::cEffectManager()
{
}


cEffectManager::~cEffectManager()
{

	//while (!m_vecEffectTobeDeleted.empty()){
	//	cEffect* p = m_vecEffectTobeDeleted.back();
	//	m_vecEffectTobeDeleted.pop_back();
	//	m_setEffects.erase(p);
	//	SAFE_RELEASE(p);
	//}

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

	while (!m_qeueuHPPotionEffectPool.empty()){
		cEffect* p = m_qeueuHPPotionEffectPool.front();
		m_qeueuHPPotionEffectPool.pop();
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
	for (UINT i = 0; i < 10; i++){
		cEffectPotion* p = new cEffectPotion;
		m_qeueuHPPotionEffectPool.push(p);
		p->SetOwner(this);
	}
	for (UINT i = 0; i < 50; i++){
		cEffectSkill1* p = new cEffectSkill1;
		p->SetOwner(this);
		m_qeueuSkill1EffectPool.push(p);
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

	if (!m_vecEffectTobeDeleted.empty()){
		for (auto p : m_vecEffectTobeDeleted){
			m_setEffects.erase(p);
			if (p->GetEffectType() == p->E_EFFECT_ONHIT){
				m_qeueuOnGetHitEffectPool.push(p);
			}
			else if (p->GetEffectType() == p->E_EFFECT_HPPOTION){
				m_qeueuHPPotionEffectPool.push(p);
			}
			else if (p->GetEffectType() == p->E_EFFECT_MPPOTION){
				m_qeueuHPPotionEffectPool.push(p);
			}
			else if (p->GetEffectType() == p->E_EFFECT_SKILL1){
				m_qeueuSkill1EffectPool.push(p);
			}
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
	else if (uiType == cEffect::E_EFFECT_HPPOTION){
		if (m_qeueuHPPotionEffectPool.empty()){
			cEffectPotion* p = new cEffectPotion;
			p->Setup();
			p->SetPosition(vPos);
			p->SetOwner(this);
			m_setEffects.insert(p);
		}
		else{
			cEffect* p = m_qeueuHPPotionEffectPool.front();
			m_qeueuHPPotionEffectPool.pop();
			p->Setup();
			p->SetPosition(vPos);
			m_setEffects.insert(p);
		}
	}
	else if (uiType == cEffect::E_EFFECT_MPPOTION){
		if (m_qeueuHPPotionEffectPool.empty()){
			cEffectPotion* p = new cEffectPotion;
			p->SetEffectType(p->E_EFFECT_MPPOTION);
			p->Setup();
			p->SetPosition(vPos);
			p->SetOwner(this);
			m_setEffects.insert(p);
		}
		else{
			cEffect* p = m_qeueuHPPotionEffectPool.front();
			m_qeueuHPPotionEffectPool.pop();
			p->SetEffectType(p->E_EFFECT_MPPOTION);
			p->Setup();
			p->SetPosition(vPos);
			m_setEffects.insert(p);
		}
	}
	else if (uiType == cEffect::E_EFFECT_SKILL1){
		if (m_qeueuSkill1EffectPool.empty()){
			cEffectSkill1* p = new cEffectSkill1;
			p->SetEffectType(p->E_EFFECT_SKILL1);
			p->Setup();
			p->SetPosition(vPos);
			p->SetOwner(this);
			m_setEffects.insert(p);
		}
		else{
			cEffect* p = m_qeueuSkill1EffectPool.front();
			m_qeueuSkill1EffectPool.pop();
			p->SetEffectType(p->E_EFFECT_SKILL1);
			p->Setup();
			p->SetPosition(vPos);
			m_setEffects.insert(p);
		}
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