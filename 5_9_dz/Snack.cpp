#include "Snack.h"
namespace SnackMachine
{
	using  namespace std;
	Snack::Snack()
	{
		setParams("Empty", 0.f, 0.f, 0);		
	}
	Snack::Snack(const string n, const float p, const float w, const int s)
	{
		setParams(n, p, w, s);
	}
	Snack::Snack(const Snack& o)
	{
		setParams(o.getName(),o.getPrice(),o.getWeight(), o.getSize());
	}
	void Snack::setParams(const string &n, const float &p, const float &w, const int &s)
	{
		name = n;
		price = p;
		weight = w;
		size = s;
	}
	Snack::~Snack()
	{
		//hmmm...
	}
	string Snack::getName() const
	{
		return name;
	}
	float Snack::getPrice() const
	{
		return price;
	}
	float Snack::getWeight() const
	{
		return weight;
	}
	int Snack::getSize() const
	{
		return size;
	}
	string Snack::preview() const
	{
		return name + " " + to_string(price) + "p.";
	}
}
