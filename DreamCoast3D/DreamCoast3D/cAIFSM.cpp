#include "stdafx.h"
#include "cAIFSM.h"

void cAIIdle::Start(cGameAIObject* pAIObject){

}
void cAIIdle::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIIdle::Exit(cGameAIObject* pAIObject){

}

int  cAIIdle::GetCurrentStateType(){
	return 0;
}

void cAIMove::Start(cGameAIObject* pAIObject){

}
void cAIMove::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIMove::Exit(cGameAIObject* pAIObject){

}
int  cAIMove::GetCurrentStateType(){
	return 1;
}

void cAIAttack::Start(cGameAIObject* pAIObject){

}
void cAIAttack::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIAttack::Exit(cGameAIObject* pAIObject){

}
int  cAIAttack::GetCurrentStateType(){
	return 2;
}

void cAIOnHit::Start(cGameAIObject* pAIObject){

}
void cAIOnHit::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIOnHit::Exit(cGameAIObject* pAIObject){

}
int  cAIOnHit::GetCurrentStateType(){
	return 3;
}

void cAIThink::Start(cGameAIObject* pAIObject){

}
void cAIThink::Execute(cGameAIObject* pAIObject, float fDelta){

}
void cAIThink::Exit(cGameAIObject* pAIObject){

}
int  cAIThink::GetCurrentStateType(){
	return 4;
}