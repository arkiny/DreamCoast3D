#include "stdafx.h"
#include "cItemConsume.h"


cItemConsume::cItemConsume()
	: m_eConsumeType(E_CONSUME_TYPE::E_CONSUME_HP_UP)
	, m_fItemCoolTime(.5f)
	, m_nCount(10)
{
}


cItemConsume::~cItemConsume()
{
}

void cItemConsume::Activate(
	iGameObjectDelegate* pGameObjgDeligate,
	iEffectManagerDelegate* pEffectDelegate,
	iSceneDelegate* pSceneDeligate,
	iUIManagerDeligate* pUIManagerDeligate,
	iGameEventDelegate* pEventDelegate
	)
{
	if (pGameObjgDeligate){
		if (m_eConsumeType == E_CONSUME_HP_UP){
			//pGameObjgDeligate->GetPlayerStatInfo()->fCurrentHp += 10;
			if (pGameObjgDeligate->GetPlayerStatInfo()->fMaxHp > pGameObjgDeligate->GetPlayerStatInfo()->fCurrentHp
				&& m_nCount > 0){
				pGameObjgDeligate->GetPlayerStatInfo()->fCurrentHp += 10;
				m_nCount--;
				D3DXVECTOR3 pos = pGameObjgDeligate->GetPlayerData()->GetPosition();
				pos.y = pos.y + 1.0f;
				pEffectDelegate->AddEffect(3, pos);
				g_pSoundManager->ExecuteEffect(std::string("../Resources/Sounds/Effect/potion.mp3"));
			}
		}
		if (m_eConsumeType == E_CONSUME_MP_UP){
			//pGameObjgDeligate->GetPlayerStatInfo()->fCurrentHp += 10;
			if (pGameObjgDeligate->GetPlayerStatInfo()->fMaxMana > pGameObjgDeligate->GetPlayerStatInfo()->fCurrentMana
				&& m_nCount > 0){
				pGameObjgDeligate->GetPlayerStatInfo()->fCurrentMana += 10;
				m_nCount--;
				D3DXVECTOR3 pos = pGameObjgDeligate->GetPlayerData()->GetPosition();
				pos.y = pos.y + 1.0f;
				pEffectDelegate->AddEffect(4, pos);
			}
		}
	}
}

std::string cItemConsume::GetTexturePath(){
	if (m_eConsumeType == E_CONSUME_HP_UP){
		return std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga");
	}
	else if (m_eConsumeType == E_CONSUME_MP_UP){
		return std::string("../Resources/ICON/ICON_Item/MP_Potion_Level_33_Tex.tga");
	}
	return std::string("");
}