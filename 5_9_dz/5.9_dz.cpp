#include <iostream>
#include "SMachine.h"

using namespace std;
using namespace SnackMachine;

int main()
{	
	SMachine* m = new SMachine();	


	SCase* c = new SCase(2,5,7);
	c->insert(new Snack("Bounty",100,0.4f,1));
	c->insert(new Snack("Bounty", 100, 0.4f, 1));
	c->insert(new Snack("Mars", 200, 0.8f, 1));
	c->insert(new Snack("BountyXL", 150, 0.9f, 2));
	c->insert(new Snack("BountyXL", 150, 0.9f, 2));
	c->insert(new Snack("BountyXL", 150, 0.9f, 2));
	if (!m->insertCase(c, 1)) delete c;
	cout << endl;

	c = new SCase(1, 3, 10);
	c->insert(new Snack("Mars", 200, 0.4f, 1));
	c->insert(new Snack("Mars", 200, 0.4f, 1));
	c->insert(new Snack("GoldIngot", 22000, 11.f, 4));
	c->insert(new Snack("Bounty", 100, 0.4f, 1));
	if (!m->insertCase(c, 2)) delete c;
	cout << endl;

	c = new SCase(1, 3, 10);
	c->insert(new Snack("Mars", 200, 0.4f, 1));
	c->insert(new Snack("Mars", 200, 0.4f, 1));
	c->insert(new Snack("Bounty", 100, 0.4f, 1));
	if (!m->insertCase(c, 0)) delete c;
	cout << endl;

	c = nullptr;

	Snack* s = nullptr;
	while (m->Interact(&s))
	{
		if (s)
		{
			//om-nom-nom
			cout << "------------------------>" << s->getName() << " eaten." << endl;
			delete s;
			s = nullptr;			
		}
	}

	delete m;
	if(s) delete s;
	
}