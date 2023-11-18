#pragma once
#include <string>
#include <iostream>
namespace SnackMachine
{
	using  namespace std;
	class Snack
	{
	public:
		 Snack();
		 Snack(const string n, const float p, const float w, const int s);
		 Snack(const Snack& o);
		~Snack();

		string	getName() const;
		float	getPrice() const;
		float	getWeight() const;
		int		getSize() const;

		string preview() const;

	private:
		string	name;
		float	price;
		float	weight;
		int		size;

		void setParams(const string &n, const float &p, const float &w, const int &s);
	};
}

