#include "stdafx.h"
#include "cGameMapObject.h"


cGameMapObject::cGameMapObject()
	: cGameObject()
{
	m_nIndex = -1;
	memset(&m_arrAdjMapIndex, -1, sizeof(int) * ARRAYSIZE(m_arrAdjMapIndex));
}

cGameMapObject::~cGameMapObject()
{
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