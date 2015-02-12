#pragma once

class cASEInstance;
class cGameASEObject : public cGameObject
{
protected:
	cASEInstance*				m_pASEInstance;
public:
	cGameASEObject();
	~cGameASEObject();

	virtual void Setup(std::string sFolder, std::string sFile);
	virtual void Update(float fDelta);
	virtual void Render();
};

