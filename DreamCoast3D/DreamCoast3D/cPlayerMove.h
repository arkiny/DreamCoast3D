#pragma once
#include "iPlayerState.h"

class cPlayerMove : public iPlayerState
{
public:
	cPlayerMove();
	~cPlayerMove();

	void Start(cGamePlayableObject* pPlayer);
	void Execute(cGamePlayableObject* pPlayer, float fDelta);
	void Exit(cGamePlayableObject* pPlayer);
	int GetCurrentStateType() { return 1; }

	// MS
private:
	bool	m_isMouseMove;
	POINT	m_ptPrevMouse;
	float	m_fAngleX;
	float	m_fAngleY;

	void MouseUpdate(cGamePlayableObject* pPlayer);
};

