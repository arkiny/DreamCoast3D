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
class cUISkillShortCut : public cUIObject, public iShortCutDragDropDeligate
{
protected:
	std::vector<cUISkillIcon*> m_vecSkills;

public:
	cUISkillShortCut();
	~cUISkillShortCut();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	// Overide iShortCutDragCropDelgate
	virtual void AddShortCutIntoBox(cUISkillIcon* pSkill) override;
};

