#pragma once

__interface iInventoryControl{
	void	UpdateInventoryItemCount(int nIndex, int nCount);// 0 체력 1 마나
	int		GetInventoryItemCount(int nIndex);
};

