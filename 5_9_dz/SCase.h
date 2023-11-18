#pragma once
#include <string>
#include <iostream>
#include "Snack.h"

namespace SnackMachine
{
	using  namespace std;
	class SCase
	{
	public:
		SCase();
		SCase(const int SSize, const int SCount, const float MaxWeight);
		~SCase();

		void insert(Snack* item);
		int getFreeSlotCount();
		bool haveSnack();
		string SnackPreview();
		float SnackPrice();
		Snack* getSnack();

	private:
		int slotSize;
		int slotCount;
		float maxWeight;
		float curWeight;

		int icount;
		Snack** items;

		void setParams(const int SSize, const int SCount, const float MaxWeight);
	};	
}

