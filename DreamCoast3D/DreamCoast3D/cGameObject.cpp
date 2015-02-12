#include "stdafx.h"
#include "cGameObject.h"
#include "cTransform.h"

cGameObject::cGameObject()
	:m_stBoundingSphere()
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
	// TODO :
	// 만약 모든 게임 오브젝트가 공통적으로 업데이트가 필요할 경우
}

void cGameObject::Render(){
	// TODO :
	// 만약 모든 게임 오브젝트가 공통적으로 렌더 시 수행하는 것이 있을 경우
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());

}


ST_BOUNDING_SPHERE*	cGameObject::GetBoundingSphere(){
	return &m_stBoundingSphere;
}

cTransform*	cGameObject::GetTransform(){
	return m_pTransform;
}

D3DXMATRIXA16* cGameObject::GetTransformMatrix(){
	return m_pTransform->GetTransformMatrix();
}

void cGameObject::SetPosition(D3DXVECTOR3& newPos){
	//D3DXVECTOR3 pos = m_pTransform->GetPosition();
	//float diff = (m_stBoundingSphere.m_vCenter.y - pos.y) * m_pTransform->GetScaling().y;
	m_pTransform->SetPosition(newPos);
	m_stBoundingSphere.m_vCenter = newPos;
	//m_stBoundingSphere.m_vCenter.x = newPos.x;
	//m_stBoundingSphere.m_vCenter.y = newPos.y + diff;
	//m_stBoundingSphere.m_vCenter.z = newPos.z;
}

D3DXVECTOR3& cGameObject::GetPosition(){
	return m_pTransform->GetPosition();
}

void cGameObject::SetYPosition(float y){
	D3DXVECTOR3 curPos = m_pTransform->GetPosition();
	m_pTransform->SetPosition(D3DXVECTOR3(curPos.x, y, curPos.z));
	m_stBoundingSphere.m_vCenter = D3DXVECTOR3(curPos.x, m_stBoundingSphere.m_vCenter.y, curPos.z);
}

void cGameObject::SetScale(D3DXVECTOR3& newScale){
	m_pTransform->SetScaling(newScale);
	m_stBoundingSphere.m_fRadius *= newScale.x;
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