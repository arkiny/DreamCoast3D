#pragma once

__interface iInventoryControl{
	void	UpdateInventoryItemCount(int nIndex, int nCount);// 0 ü�� 1 ����
	int		GetInventoryItemCount(int nIndex);
};

