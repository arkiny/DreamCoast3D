#include "stdafx.h"
#include "cGamePlayableObject.h"
#include "cSkinnedMeshBody.h"

cGamePlayableObject::cGamePlayableObject()
{
}


cGamePlayableObject::~cGamePlayableObject()
{
}

void cGamePlayableObject::Setup(
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