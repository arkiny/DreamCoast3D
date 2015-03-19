#pragma once
#include "cUIWindow.h"

class cUITextView;

class cUIGameScore : public cUIWindow
{
protected:
	cUITextView* m_pText;

public:
	cUIGameScore();
	virtual ~cUIGameScore();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

