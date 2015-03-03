#pragma once
class cUIIcon : public cUIObject
{
protected:
	//아이콘의 종류
	//인벤토리 안에서 더블클릭/우클릭 했을때 반응이나 숏컷에서 스킬과 아이템을 구분하기 위함

	/*
	//아이템 클래스라는게 존재하고, 거기서 정해준 다음 아이콘은 그 정보를 요청해야 맞는것 같다.
	
	enum EICONTYPE
	{
		EICONTYPE_ITEM,
		EICONTYPE_EQUIPMENT,
		EICONTYPE_SKILL,
		EICONTYPE_MAX
	};
	EICONTYPE eIconType;
	int nNum;					//아이템/스킬 번호
	bool isStackable;			//겹쳐지는 아이템인지
	int nMaxStack;				//최대 몇개 까지 겹쳐지는지
	int nCurrStack;				//지금 몇개 겹쳐졌는지
	*/
	bool m_isClick;				//클릭 범위에서 마우스가 클릭 되었는가
	bool m_isFloating;			//마우스에 붙어 있는 중인가(드래그 중인가)
public:
	cUIIcon();
	~cUIIcon();
};

