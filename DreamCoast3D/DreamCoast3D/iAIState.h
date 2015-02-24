#pragma once

class cGameAIObject;

__interface iAIState
{
	void Start(cGameAIObject* pAIObject);
	void Execute(cGameAIObject* pAIObject, float fDelta);
	void Exit(cGameAIObject* pAIObject);
	int GetCurrentStateType();
};