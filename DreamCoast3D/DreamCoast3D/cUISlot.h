#pragma once
#include "cUIImageView.h"
#include "cUIIcon.h"

class cDataItem;

class cUISlot : public cUIImageView
{
protected:
	SYNTHESIZE(int, m_nNum, Num);			//자기자신의 번호(컨테이너 인덱스와 동일)
	SYNTHESIZE(cUIIcon*, m_pIcon, Icon);
	SYNTHESIZE(int, m_nItemNum, ItemNum);	//자기가 가지고 있는 아이템 번호(벡터 찾기용) //HACK: map이면 훨씬 편하겠다.
	SYNTHESIZE(cDataItem*, m_pItem, Item);	//자기가 가지고 있는 아이템 포인터
public:
	cUISlot();
	cUISlot(LPD3DXSPRITE pSprite);
	~cUISlot();
	//virtual void Update(float fDelta);
	virtual void Render();
};