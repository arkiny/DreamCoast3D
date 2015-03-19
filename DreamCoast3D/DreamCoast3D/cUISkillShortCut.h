#pragma once

class cUISkillIcon;
class cUIImageView;
class cItemWithEffect;
class cUITextView;

// TODO �ӽ÷� �������� �������̽� ���� �۾��� �߰�
__interface iShortCutDragDropDeligate{
	void AddShortCutIntoBox(cUISkillIcon* pSkill);
};

// TODO
// �Ŀ� �÷��̾�� ĳ���Ͱ� ��������Ʈ�� ������ ������,
// ��ų ���� ��������Ʈ�� �̿��� ó��
// Ȥ�� ��ų ���� ��Ÿ���� �ݹ����� ó��
// ����� �׷����⸸ �� UI, ���� �浹��������
// ��üũ�ؼ� �巡�׾ص�ӿ� �°� �����ؾ��Ѵ�.
class cUISkillShortCut : public cUIWindow, public iShortCutDragDropDeligate, public iInventoryControl
{
protected:
	cUIObject*						m_pUIRoot;

	std::vector<cUIImageView*>		m_vecSlots;
	std::vector<cUITextView*>		m_vecItemNums;

	std::vector<cUIImageView*>		m_vecShadows;

	std::vector<cItemWithEffect*>	m_vecItems;
	std::vector<float>				m_vecDelayCoolTime;

public:
	cUISkillShortCut();
	virtual ~cUISkillShortCut();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// Overide iShortCutDragCropDelgate
	virtual void AddShortCutIntoBox(cUISkillIcon* pSkill) override;

	void	UpdateInventoryItemCount(int nIndex, int nCount) override;
	int		GetInventoryItemCount(int nIndex) override;
};

