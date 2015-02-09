#pragma once
__interface iMap{
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) = 0;
};
