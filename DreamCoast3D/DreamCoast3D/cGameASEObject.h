#pragma once

class cASEInstance;
class cGameASEObject : public cGameObject
{
protected:
	SYNTHESIZE(std::string, m_sMapFilePath, MapFilePath);
	SYNTHESIZE(std::string, m_sMapFolderPath, MapFolderPath);
	cASEInstance*				m_pASEInstance;
	LPD3DXMESH					m_pDebugBoxMesh;

	std::string					m_sASEFolder;
	std::string					m_sASEFile;


public:
	cGameASEObject();
	~cGameASEObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	void Update(float fDelta);
	virtual void Render();
	virtual void RenderShadow() override;

	virtual ST_BOUNDING_BOX* GetBoundingBox() override;
	virtual void Clone(cGameObject** pTarget) override;

	std::string SaveAsStringInfo() override;
};

