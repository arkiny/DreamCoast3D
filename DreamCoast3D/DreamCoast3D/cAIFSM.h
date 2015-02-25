#pragma once
#include "iAIState.h"

class cAIIdle : public iAIState{
	void Start(cGameAIObject* pAIObject);
	void Execute(cGameAIObject* pAIObject, float fDelta);
	void Exit(cGameAIObject* pAIObject);
	int GetCurrentStateType();
};

class cAIMove : public iAIState{
	virtual void Start(cGameAIObject* pAIObject);
	virtual void Execute(cGameAIObject* pAIObject, float fDelta);
	virtual void Exit(cGameAIObject* pAIObject);
	virtual int GetCurrentStateType();
};

class cAIRandomMove : public cAIMove{
	virtual void Start(cGameAIObject* pAIObject);
	virtual void Execute(cGameAIObject* pAIObject, float fDelta);
	virtual void Exit(cGameAIObject* pAIObject);
	virtual int GetCurrentStateType();
};

class cAIAttack : public iAIState{
	virtual void Start(cGameAIObject* pAIObject);
	virtual void Execute(cGameAIObject* pAIObject, float fDelta);
	virtual void Exit(cGameAIObject* pAIObject);
	virtual int GetCurrentStateType();
};

class cAIOnHit : public iAIState{
	virtual void Start(cGameAIObject* pAIObject);
	virtual void Execute(cGameAIObject* pAIObject, float fDelta);
	virtual void Exit(cGameAIObject* pAIObject);
	virtual int GetCurrentStateType();
};

class cAIThink : public iAIState{
	virtual void Start(cGameAIObject* pAIObject);
	virtual void Execute(cGameAIObject* pAIObject, float fDelta);
	virtual void Exit(cGameAIObject* pAIObject);
	virtual int GetCurrentStateType();
};