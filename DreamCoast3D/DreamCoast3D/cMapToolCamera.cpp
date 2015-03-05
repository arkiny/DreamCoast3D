#include "stdafx.h"
#include "cMapToolCamera.h"


cMapToolCamera::cMapToolCamera()
	: m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_fAngleX(0.0f)
	, m_fAngleY(0.0f)
	, m_isMButtonDown(false)
	, m_pvTarget(NULL)
	, m_fDist(1.0f)
	, m_fMin(5.0f)
	, m_fMax(400.0f)
{
	ZeroMemory(m_vFocus, sizeof(D3DXVECTOR3));
	m_fFocusSpeed = 10.f;
	m_fFocusAngleX = 0.f;
	m_fFocusAngleY = 0.f;

	//
	m_vEye = { 125, 10, 125 };
}

cMapToolCamera::~cMapToolCamera()
{
}

void cMapToolCamera::Setup()
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

void cMapToolCamera::Update()
{
	KeyBoradUpdate();

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cMapToolCamera::KeyBoradUpdate()
{
	float fDelta = g_pTimer->DeltaTime();

	if (g_pControlManager->GetInputInfo('A'))
	{
		m_fFocusAngleX += 0.05f;
	}

	if (g_pControlManager->GetInputInfo('D'))
	{
		m_fFocusAngleX -= 0.05f;
	}

	D3DXVECTOR3 vWayPoint(0.f, 0.f, 0.f);
	vWayPoint.x = cosf(m_fFocusAngleX);
	vWayPoint.y = sinf(-m_fFocusAngleY);
	vWayPoint.z = sinf(m_fFocusAngleX);
	D3DXVec3Normalize(&vWayPoint, &vWayPoint);

	if (g_pControlManager->GetInputInfo('W'))
	{
		m_vEye += vWayPoint;
	}

	if (g_pControlManager->GetInputInfo('S'))
	{
		m_vEye -= vWayPoint;
	}

	D3DXVECTOR3 vLeft(0.f, 0.f, 0.f);
	D3DXVECTOR3 vRight(0.f, 0.f, 0.f);

	vLeft.x = cosf(m_fFocusAngleX + D3DX_PI / 2);
	vLeft.z = sinf(m_fFocusAngleX + D3DX_PI / 2);
	D3DXVec3Normalize(&vLeft, &vLeft);

	vRight.x = cosf(m_fFocusAngleX - D3DX_PI / 2);
	vRight.z = sinf(m_fFocusAngleX - D3DX_PI / 2);
	D3DXVec3Normalize(&vRight, &vRight);

	if (g_pControlManager->GetInputInfo('Q'))
	{
		m_vEye += vLeft;
	}

	if (g_pControlManager->GetInputInfo('E'))
	{
		m_vEye += vRight;
	}

	if (g_pControlManager->GetInputInfo('R'))
	{
		m_vEye.y += 1.f;
	}

	if (g_pControlManager->GetInputInfo('F'))
	{
		m_vEye.y -= 1.f;
	}
	MouseUpdate();
	m_vLookAt = m_vEye + vWayPoint;	
}

void cMapToolCamera::MouseUpdate()
{
	D3DXVECTOR3 vWayPoint(0.f, 0.f, 0.f);
	vWayPoint.x = cosf(m_fFocusAngleX);
	vWayPoint.z = sinf(m_fFocusAngleX);
	D3DXVec3Normalize(&vWayPoint, &vWayPoint);

	if (g_pControlManager->GetInputInfo(VK_MBUTTON) && m_isMButtonDown == false)
	{
		m_ptPrevMouse = g_pControlManager->GetCurrentCursorPosition();
		m_isMButtonDown = true;
	}

	if (g_pControlManager->GetInputInfo(VK_MBUTTON) == false && m_isMButtonDown == true)
	{
		m_isMButtonDown = false;
	}

	if (g_pControlManager->GetInputInfo(VK_MBUTTON) && m_isMButtonDown == true)
	{
		POINT ptCurrMouse;
		GetCursorPos(&ptCurrMouse);
		ScreenToClient(g_hWnd, &ptCurrMouse);

		float fDeltaX = -(ptCurrMouse.x - m_ptPrevMouse.x) / 100.f;
		float fDeltaY = (ptCurrMouse.y - m_ptPrevMouse.y) / 100.f;

		m_fFocusAngleX += fDeltaX;
		m_fFocusAngleY += fDeltaY;

		m_ptPrevMouse = ptCurrMouse;
	}

	D3DXMATRIXA16 matRotY;
	D3DXMatrixRotationY(&matRotY, m_fAngleX);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matRotY);
}