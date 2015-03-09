#pragma once

class cGameObject;

__interface iGameEventDelegate
{
	virtual void CheckEventFromRange(cGameObject* pFrom, int nRange);
	virtual void SetCurrentTileSystem(iGridTileSystem* pGridSystem);
};