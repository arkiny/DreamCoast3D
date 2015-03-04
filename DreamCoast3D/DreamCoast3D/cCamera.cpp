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
	, m_nRunout(0)
	, m_pMap(NULL)
{
	//m_fDist = D3DXVec3Length(&m_vEye);

	// MS
    m_fFixedAngleX = 0.f;
    m_fFixedAngleY = 0.f;
	m_fDirection = 0.f;
	m_isMove = false;
    m_isAttack = false;
    m_fPassTime = 0.2f;
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
    if (g_pControlManager->GetInputInfo(VK_CONTROL))
    {
        m_isAttack = true;
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
	m_vLookAt.y = m_pvTarget->y - m_fDist;

	D3DXMatrixIdentity(&matY);
	D3DXMatrixRotationY(&matY, -D3DX_PI / 2);
	D3DXVec3TransformCoord(&m_vEye, &pvTarget, &matY);
	m_vEye += *m_pvTarget;
	m_vEye.y = m_pvTarget->y + m_fDist;


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
		m_vLookAt.y += 1.0f;
	}

	if (m_pMap){
		bool isLand;
		float fHeight = m_pMap->GetHeight(isLand, &m_vEye);

		if (m_vEye.y < fHeight + m_fMin){
			m_vEye.y = fHeight + m_fMin;
		}
	}
	D3DXVECTOR3 dist = m_vEye - m_vLookAt;
	float fAfterDist = D3DXVec3Length(&dist);
    if (m_isAttack)
    {
        AttackCameraMoving();
    }

    D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);

}

void cCamera::SetTarget(D3DXVECTOR3* pvTarget)
{
	m_pvTarget = pvTarget;
}

void cCamera::UpdateAngle(float fAngle)
{
	m_fDirection = fAngle;
}

void cCamera::AttackCameraMoving()
{
    //{
    //    float fAttackTime = 0.2f;
    //    float nCameraMove = 0.05f;
    //    float fDist = 0.2f;
    //    float fShake = 0;
    //    float fPower = 0;
    //    float fTimeInterval = 0.1f;

    //    m_fPassTime -= g_pTimer->DeltaTime();
    //    
    //    if (m_fPassTime < 0.0f)
    //    {
    //        m_isAttack = false;
    //        m_fPassTime = fAttackTime;
    //    }
    //    fShake = m_fPassTime / fTimeInterval;

    //    if (fShake > 1)
    //    {
    //        m_vEye.x += nCameraMove;
    //        m_vLookAt.x += nCameraMove;
    //        m_vEye.z -= nCameraMove;
    //        m_vLookAt.z -= nCameraMove;


    //    }
    //    else
    //    {
    //        m_vEye.x -= nCameraMove;
    //        m_vLookAt.x -= nCameraMove;
    //        m_vEye.z += nCameraMove;
    //        m_vLookAt.z += nCameraMove;
    //    }
    //}

	float nCameraMove = 0.05f;
	while (m_nRunout < 2)
	{
		m_nRunout += 1;

		if (m_nRunout == 1)
		{
			m_vEye.x += 3;
			m_vLookAt.x += 3;
			m_vEye.z -= 3;
			m_vLookAt.z -= 3;
		}

		if (m_nRunout == 2)
		{
			m_vEye.x -= 3;
			m_vLookAt.x -= 3;
			m_vEye.z += 3;
			m_vLookAt.z += 3;
		}
	}
}
