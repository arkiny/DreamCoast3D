#pragma once

#include "cLoader.h"

class cASEInstance;
class cMtlTex;

class cAseLoader : public cLoader
{
private:
	FILE*		m_fp;
	char		m_szToken[1024];
	cASEInstance*		m_pRootFrame;
	std::string	m_sFolder;

	std::vector<cMtlTex*>			m_vecMtlTex;
	std::map<std::string, cASEInstance*>	m_mapFrame;

public:
	cAseLoader(void);
	~cAseLoader(void);

	cASEInstance* Load(std::string& sFolder, std::string& sFileName);

private:
	char*	GetToken();
	bool	IsWhite(char c);
	bool	IsEqual(char* szStr1, char* szStr2);
	int		GetInteger();
	float	GetFloat();
	void	SkipBlock();
	
	void	ProcessMaterialList();
	void	ProcessMaterial(cMtlTex* pMtlTex);
	void	ProcessMapDiffuse(cMtlTex* pMtlTex);
	cASEInstance*	ProcessGeomObject();
	void	ProcessNodeTM(cASEInstance* pFrame);
	void	ProcessMesh(cASEInstance* pFrame);
	void	ProcessMeshVertexList(std::vector<D3DXVECTOR3>& vecV);
	void	ProcessMeshFaceList(OUT std::vector<ST_PNT2_VERTEX>& vecVertex, IN std::vector<D3DXVECTOR3>& vecV);
	void	ProcessMeshTVertList(std::vector<D3DXVECTOR2>& vecVT);
	void	ProcessMeshTFaceList(OUT std::vector<ST_PNT2_VERTEX>& vecVertex, IN std::vector<D3DXVECTOR2>& vecVT);
	void	ProcessMeshNormals(OUT std::vector<ST_PNT2_VERTEX>& vecVertex);
	
};

