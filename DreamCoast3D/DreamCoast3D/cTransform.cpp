#include "stdafx.h"
#include "cTransform.h"

cTransform::cTransform()
	: m_vPosition(0, 0, 0),
	m_fYAxisAngle(0.0f),
	m_fZAxisAngle(0.0f),
	m_fXAxisAngle(0.0f),
	m_vScale(1.0f, 1.0f, 1.0f)
{
	D3DXMatrixIdentity(&m_matTransform);
}

cTransform::~cTransform()
{
}

D3DXMATRIXA16* cTransform::GetTransformMatrix()
{
	D3DXMATRIXA16 matT, matS, matYR, matXR, matZR;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationX(&matXR, m_fXAxisAngle);
	D3DXMatrixRotationY(&matYR, m_fYAxisAngle);
	D3DXMatrixRotationZ(&matZR, m_fZAxisAngle);
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);

	m_matTransform = matS * matXR * matYR * matZR * matT;
	return &m_matTransform;
}