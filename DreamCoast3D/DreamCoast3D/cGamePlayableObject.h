#pragma once
#include "cGameActionSkinnedMeshObj.h"

class cSkinnedMesh;

class cGamePlayableObject : public cGameActionSkinnedMeshObj
{
public:
	cGamePlayableObject();
	virtual ~cGamePlayableObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);
};

