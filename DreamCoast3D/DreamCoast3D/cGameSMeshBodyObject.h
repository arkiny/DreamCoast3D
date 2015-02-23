#pragma once
#include "cGameActionSkinnedMeshObj.h"

class cSkinnedMesh;

class cGameSMeshBodyObject : public cGameActionSkinnedMeshObj
{
public:
	cGameSMeshBodyObject();
	virtual ~cGameSMeshBodyObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);
};

