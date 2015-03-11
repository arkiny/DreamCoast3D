#include "stdafx.h"
#include "cGameMapHeightGridSystem.h"
#include "cGridSystem.h"

cGameMapHeightGridSystem::cGameMapHeightGridSystem()
{
}


cGameMapHeightGridSystem::~cGameMapHeightGridSystem()
{
	SAFE_RELEASE(m_pGridSystem);
}

void cGameMapHeightGridSystem::Setup(){
	cGameObject::Setup();
	
	m_pGridSystem = new cGridSystem;
	m_pGridSystem->Setup(m_nTileN);
}
