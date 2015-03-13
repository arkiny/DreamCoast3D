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
	static std::vector<cDataItem*> g_pDataItem;	//모든 아이템정보를 다 가지고 있을 정적 벡터 컨테이너
	SYNTHESIZE(cUIIcon*, m_pUIIcon, UIIcon);//인벤토리 안에 들어 갔을때 표시될 아이콘

	EITEMTYPE	m_eItemType;
	int			m_nItemNum;		//아이템/스킬 번호(규칙 필요)
	bool		m_isStackable;	//겹쳐지는 아이템인지
	size_t		m_nMaxStack;	//최대 몇개 까지 겹쳐지는지(기본값 5)
	size_t		m_nCurrStack;	//지금 몇개 겹쳐졌는지

public:
	cDataItem();
	cDataItem(cUIIcon* pUIIcon);
	~cDataItem();
	virtual void cDataItem::SetupItemData(EITEMTYPE	eItemType, int nItemNum, bool isStackable, size_t nMaxStack = 5);
	//virtual void cDataItem::SetupItemIcon(cUIIcon* pUIIcon);	//만일 교체된다면 이전걸 해제 해줘야 하나? : 민우
};

