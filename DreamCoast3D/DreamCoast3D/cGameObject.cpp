#include "stdafx.h"
#include "cGameObject.h"
#include "cTransform.h"

cGameObject::cGameObject() 
	: m_vCenter(0,0,0)
	, m_fRadius(0.0f)
{
	m_pTransform = new cTransform;
}


cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pTransform);
}

void cGameObject::Setup(){

}

void cGameObject::Update(float fdelta){
	// @TODO
	// 만약 모든 게임 오브젝트가 공통적으로 업데이트가 필요할 경우
}

void cGameObject::Render(){
	// @TODO
	// 만약 모든 게임 오브젝트가 공통적으로 렌더 시 수행하는 것이 있을 경우
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());

}



cTransform*	cGameObject::GetTransform(){
	return m_pTransform;
}

D3DXMATRIXA16* cGameObject::GetTransformMatrix(){
	return m_pTransform->GetTransformMatrix();
}

void cGameObject::SetPosition(D3DXVECTOR3& newPos){
	m_pTransform->SetPosition(newPos);
}

void cGameObject::SetScale(D3DXVECTOR3& newScale){
	//@TODO
}

void cGameObject::SetXangle(float fAngleRad){
	m_pTransform->SetXAxisAngle(fAngleRad);
}

void cGameObject::SetYangle(float fAngleRad){
	m_pTransform->SetYAxisAngle(fAngleRad);
}

void cGameObject::SetZangle(float fAngleRad){
	m_pTransform->SetZAxisAngle(fAngleRad);
}