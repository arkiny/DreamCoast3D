#include "stdafx.h"
#include "cGameSMeshBodyObject.h"
#include "cSkinnedMeshBody.h"

cGameSMeshBodyObject::cGameSMeshBodyObject()
{
}


cGameSMeshBodyObject::~cGameSMeshBodyObject()
{
}

void cGameSMeshBodyObject::Setup(
	std::string sFolder, std::string sFile,
	std::string sFolderHead, std::string sFileHead,
	std::string sFolderHair, std::string sFileHair
	)
{
	cSkinnedMeshBody* pSkinnedMeshBody = new cSkinnedMeshBody;
	pSkinnedMeshBody->Setup(
		sFolder, sFile,
		sFolderHead, sFileHead,
		sFolderHair, sFileHair);
	
	m_pSkinnedMesh = pSkinnedMeshBody;
}