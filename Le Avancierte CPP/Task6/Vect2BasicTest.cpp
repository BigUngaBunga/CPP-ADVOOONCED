//Ett simpelt första testprogram för Vector
//Styrs av Macrovariabler satta i TestLevel.h
//Bygger på att man steg för steg testar några funktioner i taget.
//De funktioner som inte är testas behöver inte alls finnas.
//Styrs av en "Macrovariabel" LEVEL som går från 1 .. ???
//Börja med 1 och gå sen uppåt
//Man behöver kunna se att listan har rätt innehåll
//Därför så har vi två testfunktioner

#include "TestLevel.h"

#include <cassert>
#include <iostream>
#include "Vector2.hpp"
#include "DebugPrinting.h"
using std::cout;


struct Cbasic {
	int i;
	Cbasic() = default;
	Cbasic(int i) :i(i) {}
};

#define Vect Vector2<char>
#define FOX Vector2<char> Fox("Fox");
#define FOX0 Vector2<char> Fox;
#define BAR Vector2<char> Bar("Bar");
#define BAR0 Vector2<char> Bar;

void TestBasic2() {
#if LEVEL>=1
	{

		Vector2<char> v;
		assert(v.size() == 0);
		assert(v.capacity() >= 0);
		assert(v.Invariant());
		Vector2<char> u("E");
		FinishedTest(1);
		v.push_back('E');
	}
#endif
#if LEVEL>=2
	{
		Vector2<char> fox("fox");
		assert(fox.size() == 3);
		cout << fox << std::endl;
		assert(fox == fox);
		Vector2<char> bar("bar");
		assert(!(fox == bar));
		FinishedTest(2);
	}
#endif
#if LEVEL>=3
	{
		Vector2<char> fox("fox"), bar("bar");
		assert(fox == fox && !(fox != fox));
		assert(bar < fox && !(bar > fox));
		assert(bar <= fox && !(bar >= fox));
		assert(bar <= bar && fox >= fox);
		Vector2<char> foxa("foxa");
		assert(fox<foxa&& foxa>fox);
		FinishedTest(3);

	}
#endif
#if LEVEL>=4
	{
		Vector2<char> fox("fox"), bar("bar");
		Vector2<char> fox2(fox);
		assert(fox2.Invariant() && fox2.size() == 3);
		assert(fox == fox2);
		fox2 = bar;
		assert(fox2 == bar);
		assert(fox2.size() == 3);
		assert(fox2.capacity() >= 3);
		FinishedTest(4);
	}
#endif
#if LEVEL>=5
	{
		Vector2<char> fox("fox"), bar("");
		assert(fox[0] == 'f' && fox[1] == 'o');
		fox[0] = 'g';
		assert(fox[0] == 'g' && fox[1] == 'o');
		const Vector2<char> bar2("bar");
		static_assert(std::is_const_v<std::remove_reference_t< decltype(bar2[0])>>, "fel med const i operator[]");

		fox.at(1);
		bar2.at(2);
		FinishedTest(5);
	}
#endif
#if LEVEL>=6
	{
		Vector2<char> fox("fox"), bar("");
		fox.push_back('x');
		assert(fox == "foxx");
		bar.push_back('y');
		assert(bar == "y");
		FinishedTest(6);
	}
#endif
#if LEVEL>=7
	{
		FOX BAR;
		Vect Fox2(std::move(Fox));
		assert(Fox2.Invariant() && Fox.Invariant());
		assert(Fox2 == "Fox" && Fox == "");
		Bar = std::move(Fox2);
		assert(Fox2.Invariant() && Bar.Invariant());
		assert(Bar == "Fox");
		assert(Fox2 == "" || Fox2 == "Bar"); //Tillåt swap lösning
		FinishedTest(7);
	}
#endif
#if LEVEL>=8
	{
		FOX BAR;
		Fox.reserve(100);
		assert(Fox.capacity() == 100);  //QQQ >=?
		Fox.shrink_to_fit();
		assert(Fox.capacity() == 3);
		Fox.resize(10);
		assert(Fox.capacity() >= 10);
		assert(Fox.size() == 10);
		assert(Fox[9] == 0);
		Fox.resize(3);
		assert(Fox == "Fox");
		Fox = "Fox";
		Bar = "Bar";
		swap(Fox, Bar);
		assert(Fox == "Bar" && Bar == "Fox");
		FinishedTest(8);
	}
#endif
#if LEVEL>=9
	{
		FOX BAR;
		Fox.data();
		assert(&Fox[0] == Fox.data());
		FinishedTest(9);
	}
#endif

#if LEVEL>=10
	{
		Vector2<char> fox("fox"), bar("");
		Vector2<char>::iterator it1 = fox.begin();
		Vector2<char>::iterator it2 = bar.begin();
		assert(it1 == it1 && it1 != it2);
		Vector2<char>::iterator ite1 = fox.end();
		Vector2<char>::iterator ite2 = bar.end();
		assert(it1 != ite1 && it2 == ite2);
		const Vector2<char> coo("coo");
		{
			Vector2<char>::iterator it = fox.begin(); it = fox.end();
		}
		{
			Vector2<char>::reverse_iterator it = fox.rbegin(); it = fox.rend();
		}
#if DEL == 2
		{
			Vector2<char>::const_iterator cit = fox.cbegin(); cit = fox.cend();
			cit = coo.begin(); cit = coo.end();
		}
		{
			Vector2<char>::const_reverse_iterator cit = fox.crbegin(); cit = fox.crend();
			cit = coo.rbegin(); cit = coo.rend();
		}
#endif DEL
		FinishedTest(10);
	}
#endif
#if LEVEL>=11
	{
		Vector2<char> fox("fox"), bar("");
		Vector2<char>::iterator it1 = fox.begin();
		Vector2<char>::iterator it2 = bar.begin();
		Vector2<char>::iterator it1x(it1);
		assert(it1 == it1x);

		assert(*it1 == 'f');
		*it1 = 'x';
		assert(*it1 == 'x');

		Vector2<Cbasic> clist;
		clist.push_back(Cbasic{ 1 });
		assert(clist.begin()->i == 1);
		FinishedTest(11);
	}
#endif
#if LEVEL>=12
	{
		Vector2<char> fox("fox"), bar("");
		Vector2<char>::iterator it1 = fox.begin();
		Vector2<char>::iterator it2 = bar.begin();
		assert(*++it1 == *it1);
		assert(*it1 == *it1--);
		assert(*it1 == *fox.begin());
		assert(*--it1 == *it1);
		FinishedTest(12);
	}
#endif
#if LEVEL>=13
	{
		FOX BAR;
		auto it = Fox.begin();
		auto it2 = it;
		it2 += 2;
		auto it3 = (it + 2);
		assert(it2 == it3);
		it2 = it2 - 2;
		assert(it2 == it);
		assert((it3 - it) == 2);
		FinishedTest(13);
	}
#endif
#if DEL == 2
#if LEVEL>=20
#ifdef VG_BETYG
	{
		Vector2<int> v;
		int i = 3;
		v.push_back(std::move(i));
		v.emplace_back(7);
		assert(v.size() == 2);
		assert(v[0] == 3 && v[1] == 7);
		assert(v.emplace_back(9) == 9);
	}
	{
		Vector2<Cbasic> v;
		v.emplace_back(7);
		assert(v.size() == 1);
		assert(v[0].i == 7);
	}
FinishedTest(14);
#endif VG
#endif LEVEL
#endif DEL
	
}