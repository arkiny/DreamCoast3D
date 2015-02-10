#include "stdafx.h"
#include "cZealot.h"
#include "cSkinnedMesh.h"

cZealot::cZealot()
{
}


cZealot::~cZealot()
{
}

void cZealot::SetAnimationLoop(DWORD dwIndex, bool bEnalbe){
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->SetAnimationLoop(dwIndex, bEnalbe);
	}
}

void cZealot::SetAnimation(DWORD dwindex){
	if (m_pSkinnedMesh){
		m_pSkinnedMesh->SetAnimationIndex(dwindex);
	}
}
