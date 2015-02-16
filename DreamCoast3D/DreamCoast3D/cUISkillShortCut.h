#pragma once

class cUISkillIcon;

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
class cUISkillShortCut : public cUIObject, public iShortCutDragDropDeligate
{
protected:
	RECT							m_rectClickArea;
	LPD3DXSPRITE					m_pSprite;
	cUIObject*						m_pUIRoot;
	std::vector<cUISkillIcon*>		m_vecSkills;

public:
	cUISkillShortCut();
	virtual ~cUISkillShortCut();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// Overide iShortCutDragCropDelgate
	virtual void AddShortCutIntoBox(cUISkillIcon* pSkill) override;
};

