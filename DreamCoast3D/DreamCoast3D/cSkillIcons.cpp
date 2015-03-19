#include "stdafx.h"
#include "cSkillIcons.h"


cSkillIcons::cSkillIcons()
{
}


cSkillIcons::~cSkillIcons()
{
}

void cSkillIcons::Activate(
	iGameObjectDelegate* pGameObjgDeligate,
	iEffectManagerDelegate* pEffectDelegate,
	iSceneDelegate* pSceneDeligate,
	iUIManagerDeligate* pUIManagerDeligate,
	iGameEventDelegate* pEventDelegate
	) {}

float cSkillIcons::GetCoolTime() {
	return 3.0f;
}

std::string cSkillIcons::GetTexturePath()  { 
	return std::string("../Resources/ICON/ICON_Skill/skill1.png"); 

}

int cSkillIcons::GetCount()  { 
	return 65535; 
}

void cSkillIcons::SetCount(int n) {
}