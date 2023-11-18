#include "SMachine.h"
namespace SnackMachine
{
	using  namespace std;
	SMachine::SMachine()
	{		
		money = 0.f;				
		cases = new SCase*[CASE_MAX];
		for (int i = 0; i < CASE_MAX; ++i)
			cases[i] = nullptr;
	}
	SMachine::~SMachine()
	{
		for (int i = 0; i < CASE_MAX; ++i)
		{
			delete cases[i];
			cases[i] = nullptr;
		}
		delete[] cases;
	}
	bool SMachine::insertCase(SCase* c, int slot)
	{
		if (slot > CASE_MAX || slot <= 0)
		{
			cout << "------------------------>" << "ERROR (SMachine): machine have slots 1..." << CASE_MAX <<". cant insert in slot "<< slot;
			return false;
		}
		setCase(c,slot);
		return true;
	}
	void SMachine::setCase(SCase* c, int slot)
	{
		if (cases[slot - 1]) delete cases[slot - 1];
		cases[slot - 1] = c;		
	}

	bool SMachine::Interact(Snack **pSnack)
	{		
		*pSnack = nullptr;
		PrintStatus();
		int com = 0;
		cin >> com;
		
		if (com > 0 && com <= CASE_MAX)
		{
			com--;
			if (cases[com])
			{
				if (cases[com]->haveSnack())
				{
					if (money - cases[com]->SnackPrice() > 0)
					{
						money -= cases[com]->SnackPrice();
						*pSnack = cases[com]->getSnack();
					}
					else
					{
						cout << "------------------------>" << "need more gold!" << endl;
					}
				}
				else
				{
					cout << "------------------------>" << "sorry case is empty" << endl;
				}
			}
			else
			{
				cout << "------------------------>" << "sorry case not installed" << endl;
			}
			return true;
		}
		if (com == CASE_MAX+1)
		{
			float m;
			cout << "Enter summ to add: ";
			cin >> m;
			if (m > 0)
				money += m;
			else
				cout << "------------------------>" << "ERROR: nice try."<<endl;
			return true;
		}
		if (com == CASE_MAX + 2)
		{
			cout << endl << "------------------------>" << "HAVE A NICE DAY!" << endl << endl << endl;
			return false;
		}
		
		cin.clear();
		cin.ignore('\n', 1000);
		cout << "------------------------>" << "ERROR: undefined comand" << endl;

		return true;

	}

	void SMachine::PrintStatus()
	{
		cout << "==============================================" << endl;
		cout << "Money: " << money << endl;
		cout << "Select operation:" << endl;
		for (int i = 0; i < CASE_MAX; ++i)
		{
			if (cases[i])
				cout << "  " << (i + 1) << ".  " << cases[i]->SnackPreview() << endl;
			else
				cout << "  " << (i + 1) << ".  case not installed."<< endl;
		}
		cout << "  " << (CASE_MAX + 1) << ".  insert money" << endl;
		cout << "  " << (CASE_MAX + 2) << ".  exit." << endl;
	}
}
