#include "stdafx.h"
#include "cGameObject.h"
#include "cTransform.h"

cGameObject::cGameObject()
	:m_stBoundingSphere()
	, m_pBoundingBox(NULL)
	, m_pGridCallback(NULL)
	, m_stGridPos(0, 0)
	, m_pGameObjDeligate(NULL)
	, m_eGameObjectType(eGameObjectType::E_NONE)
	, m_pEventDelegate(NULL)
{
	m_pTransform = new cTransform;
	m_pBoundingBox = new ST_BOUNDING_BOX;
	m_pBoundingBox->vMin = D3DXVECTOR3(-.5f, -.5f, -.5f);
	m_pBoundingBox->vMax = D3DXVECTOR3(.5f, .5f, .5f);
}


cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pTransform);
	//SAFE_DELETE(m_pBoundingBox);
}

void cGameObject::Setup(){
	
}

void cGameObject::Update(float fdelta){
	// TODO :
	// 만약 모든 게임 오브젝트가 공통적으로 업데이트가 필요할 경우
	
	// TODO : 유동 그리드시스템에 대한 수정보완이 필요
	if (m_pGridCallback){
		ST_TILE_GRIDPOS pos(
			static_cast<int>(floorf(m_pTransform->GetPosition().x)),
			static_cast<int>(floorf(m_pTransform->GetPosition().z))
			);

		if (pos.x != m_stGridPos.x || pos.z != m_stGridPos.z){
			m_pGridCallback->RemoveObejctOnTile(this, m_stGridPos.x, m_stGridPos.z);
			m_pGridCallback->AddObjectOnGrid(this, pos.x, pos.z);
			m_stGridPos = pos;
		}
	}
}

void cGameObject::Render(){
	// TODO :
	// 만약 모든 게임 오브젝트가 공통적으로 렌더 시 수행하는 것이 있을 경우
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
}


ST_BOUNDING_SPHERE*	cGameObject::GetBoundingSphere(){
	return &m_stBoundingSphere;
}

ST_BOUNDING_BOX* cGameObject::GetBoundingBox(){
	return m_pBoundingBox;
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
	//m_stBoundingSphere.m_vCenter.x = newPos.x;
	//m_stBoundingSphere.m_vCenter.y = newPos.y + diff;
	//m_stBoundingSphere.m_vCenter.z = newPos.z;
	/*m_stBoundingSphere.m_vCenter.y = m_stBoundingSphere.m_vCenter.y + m_stBoundingSphere.m_fRadius;*/

	m_pTransform->SetPosition(newPos);
	//m_stGridPos = ST_TILE_GRIDPOS(
	//			static_cast<int>(floorf(m_pTransform->GetPosition().x)),
	//			static_cast<int>(floorf(m_pTransform->GetPosition().z))
	//			);
	//D3DXVec3TransformCoord(&m_stBoundingSphere.m_vCenter, &m_stBoundingSphere.m_vCenter, GetTransformMatrix());
}

D3DXVECTOR3& cGameObject::GetPosition(){
	return m_pTransform->GetPosition();
}

void cGameObject::SetYPosition(float y){
	D3DXVECTOR3 curPos = m_pTransform->GetPosition();
	m_pTransform->SetPosition(D3DXVECTOR3(curPos.x, y, curPos.z));
	m_stBoundingSphere.m_vCenter = D3DXVECTOR3(curPos.x, y, curPos.z);
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

D3DXVECTOR3& cGameObject::GetScale(){
	return m_pTransform->GetScaling();
}

float cGameObject::GetYangle(){
	return m_pTransform->GetYAxisAngle();
}

void cGameObject::SetGridTileSystem(iGridTileSystem* pGrid){
	if (pGrid){
		m_pGridCallback = pGrid;
		m_stGridPos.x = static_cast<int>(floorf(m_pTransform->GetPosition().x));
		m_stGridPos.z = static_cast<int>(floorf(m_pTransform->GetPosition().z));
		pGrid->AddObjectOnGrid(this, m_stGridPos.x, m_stGridPos.z);
	}
}