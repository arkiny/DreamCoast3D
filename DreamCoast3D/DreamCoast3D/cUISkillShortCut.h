#pragma once

class cUISkillIcon;
class cUIImageView;
class cItemWithEffect;
class cUITextView;

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

