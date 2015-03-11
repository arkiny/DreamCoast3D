#pragma once

#define g_pControlManager cControlManager::GetInstance()

class cControlManager
{
	SINGLETONE(cControlManager);

private:
	bool		m_bKeyCodeArray[256];
	POINTFLOAT	m_ptfMousePos;
	POINTFLOAT	m_ptfLClickPos;
	POINTFLOAT	m_ptfRClickPos;
	POINT		m_ptMousePos;
	POINT		m_ptLClickPos;
	POINT		m_ptRClickPos;
	float		m_fWheelMove;

	bool		m_bIsMouseTrapped = true;
	RECT		m_rectMouseTrap;

public:
	bool GetInputInfo(WPARAM wParam) { return m_bKeyCodeArray[wParam]; }
	POINTFLOAT& GetCurrentCursorPositionF() { return m_ptfMousePos; }
	POINTFLOAT& GetLClickedCursorPositionF(){ return m_ptfLClickPos; }
	POINTFLOAT& GetRClickedCursorPositionF(){ return m_ptfRClickPos; }

	POINT& GetCurrentCursorPosition() { return m_ptMousePos; }
	POINT& GetLClickedCursorPosition(){ return m_ptLClickPos; }
	POINT& GetRClickedCursorPosition(){ return m_ptRClickPos; }

	float	GetWheelMoveDist(){ 
		float ret = m_fWheelMove;
		m_fWheelMove = 0.0f;
		return ret;
	}

	void Destroy();
	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

