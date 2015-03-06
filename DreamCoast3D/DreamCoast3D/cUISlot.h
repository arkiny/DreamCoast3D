#pragma once
#include "cUIImageView.h"
#include "cUIIcon.h"

class cUISlot : public cUIImageView
{
protected:
	SYNTHESIZE(cUIIcon*, m_pIcon, Icon);
public:
	cUISlot();
	~cUISlot();
};

