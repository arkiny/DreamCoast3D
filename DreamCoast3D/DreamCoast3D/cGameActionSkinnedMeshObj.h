#pragma once

class cSkinnedMesh;

class cGameActionSkinnedMeshObj : public cGameActionObject
{
protected:
	SYNTHESIZE(cSkinnedMesh*, m_pSkinnedMesh, SkinnedMesh);
	std::string				m_sFolder;
	std::string				m_sFile;

public:
	cGameActionSkinnedMeshObj();
	virtual ~cGameActionSkinnedMeshObj();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render();

	virtual ST_BOUNDING_SPHERE* GetCollisionSphere() override;
	virtual ST_BOUNDING_SPHERE* GetAttackSphere() override;
	virtual std::map<std::string, ST_BOUNDING_SPHERE>* GetUpdatedDetailedSphere() override;
	virtual void ForcedMoving(D3DXVECTOR3 vDir, float fSpeed) override;
};