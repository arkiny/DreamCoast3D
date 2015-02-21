#pragma once

#include "cGameMapHeight.h"
class cGridSystem;

class cGameMapHeightGridSystem : public cGameMapHeight
{
protected:
	cGridSystem* m_pGridSystem;

public:
	cGameMapHeightGridSystem();
	virtual 	~cGameMapHeightGridSystem();

	virtual void Setup();
	virtual void Update(float fDelta) {}
	virtual void Render() {}

	virtual void AddObjToGrid();
	virtual void RemoveObjFromGrid();
};

