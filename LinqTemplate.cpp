
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <map>

#include "IEnumerable.h"

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <queue>
#include "assert.h"
#include "linq/linq.h"

struct heavy {
	int map[1024];

	heavy() {
		for (auto& x : map)
			x = (int)(std::rand() * 10.0 / RAND_MAX);
	}

	heavy(const heavy&) = delete;

};

struct light {
	int map[1];

	light() {
		for (auto& x : map)
			x = (int)(std::rand() * 10.0 / RAND_MAX);
	}

	light(const light&) = delete;
};

template <typename T>
void bench()
{
	std::vector<T> data(20000000);
	auto enumerable = LINQ(data);
	auto x1 = test("take-vector", [&]() {
		int sum = 0;
		for (int i = 0; i < 10000; i++) {
			sum += data[i].map[0];
		}
		return sum;
	});


	auto x3 = test("take-enumerable-momo", [&]() {
		return linq::make_enumerable(data)
			.Take(10000)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});


	auto x2 = test("take-enumerable-Pratik", [&]() {
		return enumerable
			.Take(10000)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});

/*
	auto x5 = test("take-enumerable-momo-Naive", [&]() {
		return linq::make_enumerable(data)
			.Take(10000)
			.Select([](const auto& x) { return x.map[0]; })
			.Sum();
	});


	auto x4 = test("take-enumerable-Pratik-Naive", [&]() {
		return enumerable
			.Take(10000)
			.Select([](const auto& x){ return x.map[0]; })
			.Sum();
	});
*/
	assertEquals(x1, x2);
	assertEquals(x1, x3);
	//assertEquals(x1, x4);
	//assertEquals(x1, x5);

	x1 = test("where-vector", [&]() {
		int sum = 0;
		for (auto&x : data) {
			if (x.map[0] > 5)
				sum += x.map[0];
		}
		return sum;
	});


	x3 = test("where-enumerable-momo", [&]() {
		return linq::make_enumerable(data)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Where([](const auto& x) -> bool { return x > 5; })
			.Sum();
	});


	x2 = test("where-enumerable - Pratik", [&]() {
		return enumerable
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Where([](const auto& x) -> bool { return x > 5; })
			.Sum();
	});

/*
	x5 = test("where-enumerable-momo - Naive", [&]() {
		return linq::make_enumerable(data)
			.Select([](const auto& x) { return x.map[0]; })
			.Where([](const auto& x) { return x > 5; })
			.Sum();●●●●●●●●●●
	});


	x4 = test("where-enumerable - Pratik - Naive", [&]() {
		return enumerable
			.Select([](const auto& x) { return x.map[0]; })
			.Where([](const auto& x) { return x > 5; })
			.Sum();
	});
*/
	assertEquals(x1, x2);
	assertEquals(x1, x3);
	//assertEquals(x1, x4);
	//assertEquals(x1, x5);

	x1 = test("where-take-vector", [&]() {
		int sum = 0;
		int i = 0;
		for (auto&x : data) {
			if (x.map[0] > 5) {
				sum += x.map[0];
				if (++i >= 1)
					break;
			}
		}
		return sum;
	});

	x2 = test("where-take-enumerable", [&]() {
		return enumerable
			.Where([](const auto& x) -> bool { return x.map[0] > 5; })
			.Take(1)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});

	x3 = test("where-take-enumerable", [&]() {
		return linq::make_enumerable(data)
			.Where([](const auto& x) -> bool { return x.map[0] > 5; })
			.Take(1)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});

	assertEquals(x1, x2);
	assertEquals(x1, x3);

	/*x1 = test("skip-while-take-while-vector", [&]() {
		int sum = 0;
		bool step = false;
		for (auto&x : data) {
			if (!step && x.map[0] < 5)
				continue;

			step = true;

			if (x.map[0] >= 5)
				sum += x.map[0];
			else
				break;
		}
		return sum;
	});
	
	x2 = test("skip-while-take-while-enumerable", [&]() {
		return enumerable
			.SkipWhile([](const auto& x) -> bool { return x.map[0] < 5; })
			.TakeWhile([](const auto& x) -> bool { return x.map[0] >= 5; })
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});
	
	assertEquals(x1, x2);*/
}

struct SomeClass
{
	int a, b;
	SomeClass( int a, int b ) : a(a), b(b)
	{

	}
};

int main(int argc, char **argv)
{
	//std::cout << "# Light objects" << std::endl;
	//bench<light>();

	//std::cout << "# Heavy objects" << std::endl;
	//bench<heavy>();
	//testing select with field
	std::vector<SomeClass> someVec{ SomeClass(1, 10), SomeClass(2, 20), SomeClass(3, 30) };

	for (int i = 0; i < 100000; ++i)
	{
		someVec.push_back(SomeClass(rand()%1000, rand()%1000));
	}

	auto orderedBy = LINQ(someVec)
		.Where([](const auto &a)-> bool { return a.a < 10; })
		.OrderBy(&SomeClass::a, OrderByType::asc, &SomeClass::b, OrderByType::desc)
		.ToList();

	for (std::vector<SomeClass>::const_iterator ite = orderedBy.begin(); ite != orderedBy.end()-1; ++ite)
	{
		if (ite->a > (ite + 1)->a)
			std::cout << " Problems cannot verify orederby works a -> " << (ite->a) << (" > ") << (ite+1)->a << "\n" ;


		if (ite->a == (ite + 1)->a  && ite->b < (ite + 1)->b)
			std::cout << " Problems cannot verify orederby works  b -> " << (ite->b) << (" < ") <<  (ite + 1)->b << "\n";
	}
	
	return EXIT_SUCCESS;
}