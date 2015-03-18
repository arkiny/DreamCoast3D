#include "stdafx.h"
#include "cGameMapObject.h"
#include "cGridSystem.h"

cGameMapObject::cGameMapObject()
	: cGameObject(),
	m_pGridSystem(NULL)
{
	m_nIndex = -1;
	m_eGameObjectType = E_MAP;
	memset(&m_arrAdjMapIndex, -1, sizeof(int) * ARRAYSIZE(m_arrAdjMapIndex));
}

cGameMapObject::~cGameMapObject()
{
	SAFE_RELEASE(m_pGridSystem);
}

void cGameMapObject::Setup(){
	cGameObject::Setup();
}

void cGameMapObject::Update(float fDelta){
	cGameObject::Update(fDelta);
}

void cGameMapObject::Render(){
	cGameObject::Render();
}

float cGameMapObject::GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition){
	isLand = true;
	return 0.0f;
}

void cGameMapObject::AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ){
	if (m_pGridSystem){
		m_pGridSystem->AddObjectOnGrid(pGameObejct, nX, nZ);
	}
}

D3DXVECTOR3	cGameMapObject::GetObjectCenter(int nX, int nZ){
	if (m_pGridSystem){
		return m_pGridSystem->GetObjectCenter(nX, nZ);
	}
	else {
		return D3DXVECTOR3(-1, -1, -1);
	}
}

std::set<cGameObject*> cGameMapObject::GetObjectOnGrid(int nX, int nZ){
	if (m_pGridSystem){
		return m_pGridSystem->GetObjectOnGrid(nX, nZ);
	}
	else {
		return std::set<cGameObject*>();
	}
}

D3DXVECTOR3	cGameMapObject::GetTileCenterCoord(int nX, int nZ){
	if (m_pGridSystem){
		return m_pGridSystem->GetTileCenterCoord(nX, nZ);
	}
	else {
		return D3DXVECTOR3(-1, -1, -1);
	}
}

std::vector<cGameObject*> cGameMapObject::GetAdjObject(int nX, int nZ){
	if (m_pGridSystem){
		return m_pGridSystem->GetAdjObject(nX, nZ);
	}
	else {
		return std::vector<cGameObject*>();
	}
}

std::vector<cGameObject*> cGameMapObject::GetAdjObjectCustomer(int nX, int nZ, int nSize){
	if (m_pGridSystem){
		return m_pGridSystem->GetAdjObjectCustomer(nX, nZ, nSize);
	}
	else {
		return std::vector<cGameObject*>();
	}
}

std::vector<POINT*> cGameMapObject::GetAdjCircle(int nX, int nZ, int nRange)
{
	if (m_pGridSystem){
		return m_pGridSystem->GetAdjCircle(nX, nZ, nRange);
	}
	else {
		return std::vector<POINT*>();
	}
}

std::vector<cGameObject*> cGameMapObject::GetAdjObjectCircle(int nX, int nZ, int nRange){
	if (m_pGridSystem){
		return m_pGridSystem->GetAdjObjectCircle(nX, nZ, nRange);
	}
	else {
		return std::vector<cGameObject*>();
	}
}

void cGameMapObject::RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ){
	if (m_pGridSystem){
		m_pGridSystem->RemoveObejctOnTile(pGameObejct, nX, nZ);
	}
}