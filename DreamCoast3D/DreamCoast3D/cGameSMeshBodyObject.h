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
	std::string					m_sWeaponFolder;
	std::string					m_sWeaponFile;

public:
	cGameSMeshBodyObject();
	virtual ~cGameSMeshBodyObject();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);


	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair,
		std::string sFolderWeapon, std::string sFileWeapon,
		std::string sFileTexture);
};

