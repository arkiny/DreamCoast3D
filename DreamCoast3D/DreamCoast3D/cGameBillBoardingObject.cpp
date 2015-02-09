#include "stdafx.h"
#include "cGameBillBoardingObject.h"


cGameBillBoardingObject::cGameBillBoardingObject()
	: cGameObject()
	, m_vLocalPosition(0,0,0)
{
}


cGameBillBoardingObject::~cGameBillBoardingObject()
{
}

void cGameBillBoardingObject::Render(){
	D3DXMATRIXA16 matWorldTransForm = *GetTransformMatrix();
	D3DXMATRIXA16 matLocalMatrix;
	D3DXMatrixTranslation(&matLocalMatrix, m_vLocalPosition.x, m_vLocalPosition.y, m_vLocalPosition.z);

	D3DXMATRIXA16 matView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	matView._41 = matWorldTransForm._41;
	matView._42 = matWorldTransForm._42;
	matView._43 = matWorldTransForm._43;

	matView = matLocalMatrix * matView;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matView);
}