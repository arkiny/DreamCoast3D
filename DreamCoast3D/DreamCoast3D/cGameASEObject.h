#pragma once

class cASEInstance;
class cGameASEObject : public cGameObject
{
protected:
	cASEInstance*				m_pASEInstance;
	LPD3DXMESH					m_pDebugBoxMesh;
public:
	cGameASEObject();
	~cGameASEObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render();

	virtual ST_BOUNDING_BOX GetBoundingBox();
};

