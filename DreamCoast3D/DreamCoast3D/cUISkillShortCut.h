#pragma once

class cUISkillIcon;
class cUIImageView;
class cDataItem;

// TODO 임시로 만들어놓은 인터페이스 차후 작업후 추가
__interface iShortCutDragDropDeligate{
	void AddShortCutIntoBox(cUISkillIcon* pSkill);
};

// TODO
// 후에 플레이어블 캐릭터가 델리게이트를 가지고 있으며,
// 스킬 사용시 델리게이트를 이용해 처리
// 혹은 스킬 사용시 쿨타임을 콜백으로 처리
// 현재는 그려놓기만 한 UI, 차후 충돌범위등을
// 재체크해서 드래그앤드롭에 맞게 수정해야한다.
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

