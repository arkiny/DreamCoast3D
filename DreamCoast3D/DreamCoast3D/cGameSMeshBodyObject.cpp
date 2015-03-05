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

	m_sFolder = sFolder;
	m_sFile = sFile;
	m_sHeadFolder = sFolderHead;
	m_sHeadFile = sFileHead;
	m_sHairFolder = sFolderHair;
	m_sHairFile = sFileHair;
}