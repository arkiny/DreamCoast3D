#include "stdafx.h"
#include "cControlManager.h"


cControlManager::cControlManager()
	: m_fWheelMove(0)
{
	memset(m_bKeyCodeArray, 0, sizeof(m_bKeyCodeArray));
	ZeroMemory(&m_ptfMousePos, sizeof(POINTFLOAT));
	ZeroMemory(&m_ptfLClickPos, sizeof(POINTFLOAT));
	ZeroMemory(&m_ptfRClickPos, sizeof(POINTFLOAT));

	ZeroMemory(&m_ptMousePos, sizeof(POINT));
	ZeroMemory(&m_ptLClickPos, sizeof(POINT));
	ZeroMemory(&m_ptRClickPos, sizeof(POINT));
}


cControlManager::~cControlManager()
{
}

// @HACK: 민우가 인풋컨트롤에 대해선 더 나은 방법을 알고 있을수 있다.
void cControlManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_KEYDOWN:
	{
		m_bKeyCodeArray[wParam] = true;
	}
	break;
	case WM_KEYUP:
	{
		m_bKeyCodeArray[wParam] = false;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		m_ptfRClickPos.x = LOWORD(lParam);
		m_ptfRClickPos.y = HIWORD(lParam);
		m_ptRClickPos.x = LOWORD(lParam);
		m_ptRClickPos.y = HIWORD(lParam);
		m_bKeyCodeArray[VK_RBUTTON] = true;
	}
	break;
	case WM_RBUTTONUP:
	{
		m_bKeyCodeArray[VK_RBUTTON] = false;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		m_ptfLClickPos.x = LOWORD(lParam);
		m_ptfLClickPos.y = HIWORD(lParam);
		m_ptLClickPos.x = LOWORD(lParam);
		m_ptLClickPos.y = HIWORD(lParam);
		m_bKeyCodeArray[VK_LBUTTON] = true;
	}
	break;

	case WM_MBUTTONDOWN:
	{
		m_bKeyCodeArray[VK_MBUTTON] = true;
	}
	break;

	case WM_MBUTTONUP:
	{
		m_bKeyCodeArray[VK_MBUTTON] = false;
	}
	break;

	case WM_LBUTTONUP:
	{
		m_bKeyCodeArray[VK_LBUTTON] = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		m_ptfMousePos.x = LOWORD(lParam);
		m_ptfMousePos.y = HIWORD(lParam);
		m_ptMousePos.x = LOWORD(lParam);
		m_ptMousePos.y = HIWORD(lParam);
	}
	break;	
	case WM_MOUSEWHEEL:
	{
		m_fWheelMove = GET_WHEEL_DELTA_WPARAM(wParam);
	}
	break;
	}
}

void cControlManager::Destroy(){

}