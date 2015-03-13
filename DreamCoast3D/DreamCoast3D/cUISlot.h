#pragma once
#include "cUIImageView.h"
#include "cUIIcon.h"

class cDataItem;

class cUISlot : public cUIImageView
{
protected:
	SYNTHESIZE(int, m_nNum, Num);			//자기자신의 번호(컨테이너 인덱스와 동일)
	SYNTHESIZE(cUIIcon*, m_pIcon, Icon);	//보여질 아이템 아이콘
	cDataItem* m_pItem;						//슬롯에 할당되어 있는 아이템
public:
	cUISlot();
	cUISlot(LPD3DXSPRITE pSprite);
	~cUISlot();
	virtual void Update(float fDelta) override;
	virtual void Render() override;
	virtual void SetItem(cDataItem* pDataItem);
	virtual cDataItem* GetItem();
	virtual void DeleteItem();

};