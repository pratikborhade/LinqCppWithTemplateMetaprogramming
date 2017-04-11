
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

	auto x2 = test("take-enumerable", [&]() {
		return enumerable
			.Take(10000)
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Sum();
	});

	assertEquals(x1, x2);

	x1 = test("where-vector", [&]() {
		int sum = 0;
		for (auto&x : data) {
			if (x.map[0] > 5)
				sum += x.map[0];
		}
		return sum;
	});


	x2 = test("where-enumerable", [&]() {
		return enumerable
			.Select([](const auto& x) -> const int & { return x.map[0]; })
			.Where([](const auto& x) -> bool { return x > 5; })
			.Sum();
	});

	assertEquals(x1, x2);

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

	assertEquals(x1, x2);

	x1 = test("skip-while-take-while-vector", [&]() {
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
	
	assertEquals(x1, x2);
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
	std::cout << "# Light objects" << std::endl;
	bench<light>();

	/*
	std::cout << "# Heavy objects" << std::endl;
	bench<heavy>();
	//testing select with field	std::vector<SomeClass> someVec{ SomeClass(1, 10), SomeClass(2, 20), SomeClass(3, 30) };
	for (auto i : LINQ(someVec).Select(&SomeClass::a))
		std::cout << i << "\n";
	for (auto i : LINQ(someVec).Select(&SomeClass::b))		std::cout << i << "\n";	auto listb = LINQ(someVec).Select(&SomeClass::b).ToList();	for (auto i : listb)		std::cout << i;
		*/
	return EXIT_SUCCESS;
}