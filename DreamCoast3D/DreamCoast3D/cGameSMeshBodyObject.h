#pragma once
#include "cGameActionSkinnedMeshObj.h"

class cSkinnedMesh;

class cGameSMeshBodyObject : public cGameActionSkinnedMeshObj
{
protected:
	std::string					m_sHeadFolder;
	std::string					m_sHeadFile;
	std::string					m_sHairFolder;
	std::string					m_sHairFile;

public:
	cGameSMeshBodyObject();
	virtual ~cGameSMeshBodyObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);
};

