#pragma once
class cEffect;

__interface iEffectManagerDelegate
{
	virtual void AddEffect(UINT eType, D3DXVECTOR3 vPos);
};

