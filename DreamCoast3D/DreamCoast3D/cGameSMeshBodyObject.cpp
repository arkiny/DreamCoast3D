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
	m_pSkinnedMesh = new cSkinnedMeshBody(sFolder, sFile,
		sFolderHead, sFileHead,
		sFolderHair, sFileHair);
}