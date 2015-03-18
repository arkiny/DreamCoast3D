#pragma once
#include "iMap.h"

class cGridSystem;
//�ʿ��� ���� ������Ʈ�� �����̹Ƿ� ���ӿ�����Ʈ�� ����Ѵ�.
class cGameMapObject : public cGameObject, public iMap, public iGridTileSystem 
{
protected:
	SYNTHESIZE(int, m_nIndex, Index);
	int	m_arrAdjMapIndex[8]; // TODO : ���� �������ε��� ���� �����
	SYNTHESIZE(cGridSystem*, m_pGridSystem, GridSystem);

public:
	cGameMapObject();
	virtual ~cGameMapObject();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void RenderShadow(){}

	// iMap Override
	//�� ���� ���� ��� ���� �ٸ��Ƿ� ���� �������̵� ������Ѵ�.
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;
	
	virtual void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ);
	virtual D3DXVECTOR3 GetObjectCenter(int nX, int nZ);
	virtual std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);
	virtual D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);
	virtual std::vector<cGameObject*> GetAdjObject(int nX, int nZ);
	virtual std::vector<cGameObject*> GetAdjObjectCustomer(int nX, int nZ, int nSize);
	virtual std::vector<cGameObject*> GetAdjObjectCircle(int nX, int nZ, int nRange);
	virtual void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);

	virtual std::vector<ST_PNT2_VERTEX>& GetVertexVectorByRef() { return std::vector<ST_PNT2_VERTEX>(); }
	virtual void ChangeMapYVertexCoord(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax, float fDelta){}
	virtual std::vector<POINT*> GetAdjCircle(int nX, int nZ, int nRange);

};

