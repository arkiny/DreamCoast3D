#pragma once
#include "cObject.h"

class cUIIcon;

//HACK: Data라는 부모가 있어야 할 지도 모르겠다. : 민우
//HACK: 아이템 등급을 정해주는 멤버가 있으면 더 좋겠다. : 민우
class cDataItem : public cObject
{
	//아이템의 종류_
	//인벤토리 안에서 더블클릭/우클릭 했을때 반응이나 숏컷에서 스킬과 아이템을 구분하기 위함
	enum EITEMTYPE
	{
		EITEMTYPE_CONSUMABLE,	//소모품(ex 물약)
		EITEMTYPE_EQUIPMENT,	//장비(ex 갑옷)
		EITEMTYPE_ETC,			//기타(기능성이 없는 아이템)
		EITEMTYPE_MAX
	};
	SYNTHESIZE(cUIIcon*, m_pUIIcon, UIIcon);//인벤토리 안에 들어 갔을때 표시될 아이콘

	EITEMTYPE	eItemType;
	int			nItemNum;		//아이템/스킬 번호(규칙 필요)
	bool		isStackable;	//겹쳐지는 아이템인지
	int			nMaxStack;		//최대 몇개 까지 겹쳐지는지
	int			nCurrStack;		//지금 몇개 겹쳐졌는지

public:
	cDataItem();
	~cDataItem();
};

