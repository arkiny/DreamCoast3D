#pragma once
__interface iMap{
	float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition);
};

class cGameObject;

__interface iGridTileSystem{
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ);
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);
};