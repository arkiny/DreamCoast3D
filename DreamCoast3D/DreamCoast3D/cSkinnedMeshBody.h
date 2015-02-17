#pragma once
#include "cSkinnedMesh.h"


class cSkinnedMeshBody : public cSkinnedMesh
{
protected:
	cSkinnedMesh* m_pHead;
	cSkinnedMesh* m_pHair;

public:
	cSkinnedMeshBody();
	virtual ~cSkinnedMeshBody();

	virtual void Setup(
		std::string sFolder, std::string sFile,
		std::string sFolderHead, std::string sFileHead,
		std::string sFolderHair, std::string sFileHair
		);

	
	virtual void Render(D3DXMATRIXA16* pParentWorldTM);
	virtual void Render(D3DXFRAME* pFrame, D3DXMATRIXA16* pParentWorldTM);
};

