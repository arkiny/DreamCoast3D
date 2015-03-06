#include "stdafx.h"
#include "cEditCursor.h"


cEditCursor::cEditCursor()
	:m_pMesh(NULL)
{
	ZeroMemory(&m_stBox, sizeof(ST_BOUNDING_BOX));
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
}


cEditCursor::~cEditCursor()
{
	SAFE_RELEASE(m_pMesh);
}

void cEditCursor::Setup(){
	D3DXCreateBox(g_pD3DDevice, 1.0f, 1.0f, 1.0f, &m_pMesh, NULL);
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f);
}

void cEditCursor::Update(float fDelta){
	D3DXMATRIXA16 transform;
	ST_BOUNDING_BOX origin;
	origin.vMin = D3DXVECTOR3(-.5f, -.5f, -.5f);
	origin.vMax = D3DXVECTOR3(.5f, .5f, .5f);
	D3DXVec3TransformCoord(&m_stBox.vMin, &origin.vMin, GetTransformMatrix());
	D3DXVec3TransformCoord(&m_stBox.vMax, &origin.vMax, GetTransformMatrix());


}

void cEditCursor::Render(){
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, GetTransformMatrix());
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, NULL);

	if (m_pMesh){
		m_pMesh->DrawSubset(0);
	}

	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void cEditCursor::SetBoxSize(ST_BOUNDING_BOX stBox){
	m_stBox = stBox;
}

void cEditCursor::SetPosition(D3DXVECTOR3& newPos) {
	cGameObject::SetPosition(newPos);
	ST_BOUNDING_BOX origin;
	origin.vMin = D3DXVECTOR3(-.5f, -.5f, -.5f);
	origin.vMax = D3DXVECTOR3(.5f, .5f, .5f);
	D3DXVec3TransformCoord(&m_stBox.vMin, &origin.vMin, GetTransformMatrix());
	D3DXVec3TransformCoord(&m_stBox.vMax, &origin.vMax, GetTransformMatrix());
}

