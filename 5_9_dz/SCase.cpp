#include "SCase.h"
namespace SnackMachine
{
	using  namespace std;
	SCase::SCase()
	{
		setParams(1,3,1.f);
	}
	SCase::SCase(const int SSize, const int SCount, const float MaxWeight)
	{
		setParams(SSize, SCount, MaxWeight);
	}
	void SCase::setParams(const int SSize, const int SCount, const float MaxWeight)
	{
		slotSize = SSize;
		slotCount = SCount;
		maxWeight = MaxWeight;
		curWeight = 0.f;

		icount = 0;
		items = new Snack * [slotCount];
		for (int i = 0; i < slotCount; ++i)
		{
			items[i] = nullptr;
		}
	}

	SCase::~SCase()
	{
		for (int i = 0; i < icount; ++i)
		{
			delete items[i];
		}
		delete[] items;
	}
	void SCase::insert(Snack* item)
	{
		if (!item)
		{
			return;
		}
		if (icount >= slotCount)
		{
			cout << "ERROR (SCase): no space for item" << endl;
			delete item;
			return;
		}
		if(maxWeight< curWeight+item->getWeight())
		{ 
			cout << "ERROR (SCase): item" << item->getName() << " too heavy. avaible "<< (maxWeight-curWeight) << "w units." << endl;
			delete item;
			return;
		}
		if (slotSize < item->getSize())
		{
			cout << "ERROR (SCase): item too big. this case can hold sizes " << slotSize << " and less." << endl;
			delete item;
			return;
		}

		for (int i = icount; i > 0; --i)
			items[i] = items[i- 1];
		curWeight += item->getWeight();
		items[0] = item;
		icount++;

		cout << "(SCase): item \"" << items[0]->getName() << "\" inserted.  free slots: " << getFreeSlotCount() << endl;
	}		
	int SCase::getFreeSlotCount()
	{
		return slotCount - icount;
	}
	string SCase::SnackPreview()
	{
		if (items[0])
			return items[0]->preview();
		return "empty";		
	}
	Snack* SCase::getSnack()
	{
		Snack* s = items[0];
		if (s == nullptr) 
			return nullptr;

		for (int i = 0; i < icount - 1; ++i)
			items[i] = items[i + 1];
		items[icount - 1] = nullptr;
		icount--;
		return s;
	}
	bool SCase::haveSnack()
	{
		return icount > 0;
	}
	float SCase::SnackPrice()
	{
		if (icount > 0) 
			return items[0]->getPrice();
		return 0.f;
	}
}
