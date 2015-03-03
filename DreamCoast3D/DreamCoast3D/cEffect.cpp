#include "stdafx.h"
#include "cEffect.h"
#include "cTransform.h"
#include "cEffectManager.h"

cEffect::cEffect()
{
	m_pTransform = new cTransform;
}

cEffect::~cEffect()
{
	SAFE_RELEASE(m_pTransform);
}

void cEffect::Setup(){

}

void cEffect::Start(){

}

void cEffect::Update(float fDelta){

}

void cEffect::Render(){

}

void cEffect::Exit(){
	if (m_pOwner){
		m_pOwner->DeleteEffect(this);
	}
}

cTransform*			cEffect::GetTransform(){
	return m_pTransform;
}

D3DXMATRIXA16*		cEffect::GetTransformMatrix(){
	return m_pTransform->GetTransformMatrix();
}

void cEffect::SetPosition(D3DXVECTOR3 vPos){
	m_pTransform->SetPosition(vPos);
}