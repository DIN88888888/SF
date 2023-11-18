#pragma once
#include "SCase.h"
namespace SnackMachine
{
	class SMachine
	{
	public:
		const static int CASE_MAX = 3;
		SMachine();
		~SMachine();

		bool insertCase(SCase *c,int slot);
		bool Interact(Snack** pSnack);
	private:		

		float money;
		bool smode;

		int ccount;
		SCase** cases;
		void setCase(SCase *c,int slot);	
		void PrintStatus();
	};
}

