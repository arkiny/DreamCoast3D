#pragma once

class cUISkillIcon;
class cUIImageView;
class cDataItem;

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
class cUISkillShortCut : public cUIWindow, public iShortCutDragDropDeligate
{
protected:
	cUIObject*						m_pUIRoot;

	std::vector<cUIImageView*>		m_vecSlots;
	std::vector<cDataItem*>			m_vecDataInfo;

	std::vector<cUISkillIcon*>		m_vecSkills;

	std::vector<cDataItem*> m_vecItem;

public:
	cUISkillShortCut();
	virtual ~cUISkillShortCut();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// Overide iShortCutDragCropDelgate
	virtual void AddShortCutIntoBox(cUISkillIcon* pSkill) override;
};

