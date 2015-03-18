#pragma once

//iGameObjectDelegate*		m_pGameObjDelgate;
//iSceneDelegate*				m_pSceneDeligate;
//iUIManagerDeligate*			m_pUIManagerDeligate;
//iGameEventDelegate*			m_pEventDeligate;

__interface iActivatable {
	void Activate(
		iGameObjectDelegate* pGameObjgDeligate,
		iEffectManagerDelegate* pEffectDelegate,
		iSceneDelegate* pSceneDeligate,
		iUIManagerDeligate* pUIManagerDeligate,
		iGameEventDelegate* pEventDelegate
		);

	std::string GetTexturePath();
	int GetCount();
	
	float GetCoolTime();
};