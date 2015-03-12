#pragma once
#include "cUIImageView.h"

class cUIIcon : public cUIImageView
{
protected:
	//아이콘의 종류
	//인벤토리 안에서 더블클릭/우클릭 했을때 반응이나 숏컷에서 스킬과 아이템을 구분하기 위함
	enum EICONTYPE
	{
		EICONTYPE_ITEM,			//아이템 아이콘
		EICONTYPE_SKILL,		//스킬 아이콘
		EICONTYPE_MAX
	};
	SYNTHESIZE(EICONTYPE, m_eIconType, IconType);
	//bool m_isBinding;			//마우스에 붙어 있는 중인가(드래그 중인가)
public:
	cUIIcon();
	cUIIcon(LPD3DXSPRITE pSprite);
	~cUIIcon();
	virtual LPD3DXSPRITE cUIIcon::GetSprite();
	virtual void cUIIcon::SetSprite(LPD3DXSPRITE pSprite);

	virtual void Render() override;
};