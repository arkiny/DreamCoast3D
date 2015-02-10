#pragma once

class cSkinnedMesh;

class cGameActionSkinnedMeshObj : public cGameActionObject
{
protected:
	SYNTHESIZE(cSkinnedMesh*, m_pSkinnedMesh, SkinnedMesh);
	
public:
	cGameActionSkinnedMeshObj();
	virtual ~cGameActionSkinnedMeshObj();
	
	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render();
};

