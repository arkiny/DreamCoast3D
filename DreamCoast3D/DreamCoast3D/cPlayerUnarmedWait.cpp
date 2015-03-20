#include "stdafx.h"
#include "cPlayerUnarmedWait.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMesh.h"

cPlayerUnarmedWait::cPlayerUnarmedWait()
{
}


cPlayerUnarmedWait::~cPlayerUnarmedWait()
{
}
void cPlayerUnarmedWait::Start(cGamePlayableObject* pPlayer)
{
	pPlayer->GetSkinnedMesh()->SetAnimationIndex(pPlayer->EPLAYABLESTATE_UNARMEDWAIT);
}
void cPlayerUnarmedWait::Execute(cGamePlayableObject* pPlayer, float fDelta)
{

}
void cPlayerUnarmedWait::Exit(cGamePlayableObject* pPlayer)
{

}