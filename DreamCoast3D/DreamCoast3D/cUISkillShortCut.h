#pragma once

class cUISkillIcon;

// TODO 임시로 만들어놓은 인터페이스 차후 작업후 추가
__interface iShortCutDragDropDeligate{
	void AddShortCutIntoBox(cUISkillIcon* pSkill);
};

// TODO
// 후에 플레이어블 캐릭터가 델리게이트를 가지고 있으며,
// 스킬 사용시 델리게이트를 이용해 처리
// 혹은 스킬 사용시 쿨타임을 콜백으로 처리
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

