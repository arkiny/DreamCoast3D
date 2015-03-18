#pragma once

__interface iInventoryControl{
	void	UpdateInventoryItemCount(int nIndex, int nCount);
	int		GetInventoryItemCount(int nIndex);
};

