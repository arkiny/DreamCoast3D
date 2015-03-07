#pragma once
class cUIInventory;
class cUISlot;
class cDataItem;


__interface iUIInventoryDelegate
{
public:
	virtual void AddItem(cDataItem* pDataItem);
	virtual void DeleteItemInSlot(size_t nSlotNum);
	virtual void DeleteItemInSlot(cUISlot* pUISlot);
	virtual cDataItem* UseItem(cUISlot* pUISlot);
};

