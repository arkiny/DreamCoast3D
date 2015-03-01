#pragma once
#include "cAction.h"
class cActionIdle : public cAction
{
protected:
	SYNTHESIZE(float, m_fAngle, fAngle);

public:
	cActionIdle();
	virtual ~cActionIdle();

	//cAction override
	virtual void Start() override;
	virtual void Update(float delta) override;
};

