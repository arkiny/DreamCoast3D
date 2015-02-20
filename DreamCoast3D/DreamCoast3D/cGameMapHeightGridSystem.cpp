#include "stdafx.h"
#include "cGameMapHeightGridSystem.h"
#include "cGridSystem.h"

cGameMapHeightGridSystem::cGameMapHeightGridSystem()
	: m_pGridSystem(NULL)
{
}


cGameMapHeightGridSystem::~cGameMapHeightGridSystem()
{
	SAFE_RELEASE(m_pGridSystem);
}

void cGameMapHeightGridSystem::Setup(){
	m_pGridSystem = new cGridSystem;
	m_pGridSystem->Setup(m_nTileN);
}

void cGameMapHeightGridSystem::AddObjToGrid(){
	// TODO
	//m_pGridSystem->AddObjectOnGrid()
}

void cGameMapHeightGridSystem::RemoveObjFromGrid(){
	// TODO
	// m_pGridSystem->RemoveObjFromGrid()
}