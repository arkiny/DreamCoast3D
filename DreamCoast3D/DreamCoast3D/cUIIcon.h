#pragma once
#include "cUIImageView.h"

//�� Ŭ������ ��ü�� cDataItem ��ü�� ���Եǰ�, cDataItem�� ������ �� ���� �����ȴ�.
class cUIIcon : public cUIImageView
{
protected:
	//�������� ����
	//�κ��丮 �ȿ��� ����Ŭ��/��Ŭ�� ������ �����̳� ���ƿ��� ��ų�� �������� �����ϱ� ����
	enum EICONTYPE
	{
		EICONTYPE_ITEM,			//������ ������
		EICONTYPE_SKILL,		//��ų ������
		EICONTYPE_MAX
	};
	SYNTHESIZE(EICONTYPE, m_eIconType, IconType);
	//bool m_isBinding;			//���콺�� �پ� �ִ� ���ΰ�(�巡�� ���ΰ�)
public:
	cUIIcon();
	cUIIcon(LPD3DXSPRITE pSprite);
	~cUIIcon();
	static cUIIcon* CloneIcon(cUIIcon* pSrcIcon);
};