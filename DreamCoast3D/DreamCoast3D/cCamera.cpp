#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_isRButtonDown(false)
	, m_pvTarget(NULL)
	, m_fDist(5.0f)
	, m_fMin(5.0f)
	, m_fMax(400.0f)
{
	//m_fDist = D3DXVec3Length(&m_vEye);

	// MS
    m_fFixedAngleX = 0.f;
    m_fFixedAngleY = 0.f;
	m_fDirection = 0.f;
	m_isMove = false;
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup()
{
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, vp.Width / (float)vp.Height, 1.0f, 3000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Setup(
		D3DXVECTOR3& vEye,
		D3DXVECTOR3& vLookAt,
		D3DXVECTOR3& vUp,
		float fAngleX,
		float fAngleY,
		float fDist,
		float fMin,
		float fMax){
	m_vEye = vEye;
	m_vLookAt = vLookAt;
	m_vUp = vUp;
	m_fAngleX = fAngleX;
	m_fAngleY = fAngleY;
	m_fDist = fDist;
	m_fMin = fMin;
	m_fMax = fMax;

	Setup();
}

void cCamera::Update(float delta)
{
	if (g_pControlManager->GetInputInfo(VK_SPACE))
	{
		m_isMove = true;
	}
	else
	{
		m_isMove = false;
	}

	m_vLookAt = *m_pvTarget;
	
	D3DXVECTOR3 vec(0.f, 0.f, 0.f);
	vec.x = m_pvTarget->x - m_vEye.x;
	vec.z = m_pvTarget->z - m_vEye.z;

	D3DXVECTOR3 pvTarget(0.f, 0.f, 0.f);
	pvTarget.x = cos(m_fDirection);
	pvTarget.z = sin(m_fDirection);
	D3DXVec3Normalize(&pvTarget, &pvTarget);

	pvTarget *= m_fDist;

	D3DXMATRIXA16 matY;
	D3DXMatrixIdentity(&matY);
	D3DXMatrixRotationY(&matY, D3DX_PI / 2);
	D3DXVec3TransformCoord(&m_vLookAt, &pvTarget, &matY);
	m_vLookAt += *m_pvTarget;
	m_vLookAt.y = m_pvTarget->y - 20.f;

	D3DXMatrixIdentity(&matY);
	D3DXMatrixRotationY(&matY, -D3DX_PI / 2);
	D3DXVec3TransformCoord(&m_vEye, &pvTarget, &matY);
	m_vEye += *m_pvTarget;
	m_vEye.y = m_pvTarget->y + 20.f;


	if (g_pControlManager->GetInputInfo(VK_MBUTTON) && m_isRButtonDown == false){
		m_ptPrevMouse = g_pControlManager->GetCurrentCursorPosition();
		m_isRButtonDown = true;
	}

	if (g_pControlManager->GetInputInfo(VK_MBUTTON) == false && m_isRButtonDown == true){
		m_isRButtonDown = false;
	}

	if (g_pControlManager->GetInputInfo(VK_MBUTTON) && m_isRButtonDown == true){
		POINT ptCurrMouse;
		GetCursorPos(&ptCurrMouse);
		ScreenToClient(g_hWnd, &ptCurrMouse);

		float fDeltaX = (ptCurrMouse.x - m_ptPrevMouse.x) / 100.f;
		float fDeltaY = (ptCurrMouse.y - m_ptPrevMouse.y) / 100.f;

		m_fAngleX += fDeltaY;
		m_fAngleY += fDeltaX;

		if (m_fAngleX >= D3DX_PI / 2.f - 0.0001f)
			m_fAngleX = D3DX_PI / 2.f - 0.0001f;

		if (m_fAngleX <= -D3DX_PI / 2.f + 0.0001f)
			m_fAngleX = -D3DX_PI / 2.f + 0.0001f;

		m_ptPrevMouse = ptCurrMouse;
	}

	if (m_isRButtonDown == false && m_isMove == true)
	{
		float fX, fY;
		vec = m_vEye - m_vLookAt;

		fX = vec.x;
		fY = vec.z;
		m_fFixedAngleX = atan2(fY, fX);

		fX = sqrt((vec.x)*(vec.x) + (vec.z)*(vec.z));
		fY = vec.y;
		m_fFixedAngleY = atan2(fY, fX);

		m_fAngleX = m_fFixedAngleY;
		m_fAngleY = m_fFixedAngleX + D3DX_PI / 2;
	}

	float wheelMove = g_pControlManager->GetWheelMoveDist();
	if (wheelMove != 0.0f){
		m_fDist -= wheelMove / 100.f;
		if (m_fDist < m_fMin)
			m_fDist = m_fMin;
		if (m_fDist > m_fMax)
			m_fDist = m_fMax;
	}
	
	m_vEye = D3DXVECTOR3(0, 0.0f, -m_fDist);

	D3DXMATRIXA16 matTrans, transZ, matRotX, matRotY;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&transZ);
	D3DXMatrixRotationX(&matRotX, m_fAngleX);
	D3DXMatrixRotationY(&matRotY, m_fAngleY);

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRotX);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRotY);

	if (m_pvTarget)
	{
		m_vEye += (*m_pvTarget);
		m_vLookAt = (*m_pvTarget);
	}

	bool isLand;
	float fHeight = m_pMap->GetHeight(isLand, &m_vEye);
	
	if (m_vEye.y < fHeight + m_fMin){
		m_vEye.y = fHeight + m_fMin;
	}

	D3DXVECTOR3 dist = m_vEye - m_vLookAt;
	float fAfterDist = D3DXVec3Length(&dist);

    D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

}

void cCamera::SetTarget(D3DXVECTOR3* pvTarget)
{
	m_pvTarget = pvTarget;
}

//void cCamera::AdjustYPositionByHeightMap(iMap* pMap){
//	bool bIsLand = false;
//	float yPos = pMap->GetHeight(bIsLand, &m_vEye);
//	if (m_vEye.y < yPos){
//		m_vEye.y = yPos;
//	}
//}

void cCamera::UpdateAngle(float fAngle)
{
	m_fDirection = fAngle;
}
