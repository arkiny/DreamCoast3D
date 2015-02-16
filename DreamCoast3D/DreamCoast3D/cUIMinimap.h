#pragma once

class cUIMinimap : public cUIWindow
{
protected:

public:
	cUIMinimap();
	virtual ~cUIMinimap();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

