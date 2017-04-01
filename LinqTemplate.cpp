
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

template <typename T1, typename T2>
void assertEquals(T1 t1, T2 t2)
{
	if (!(t1 == t2))
	{
		std::stringstream ss;
		ss << "Assertion failed: ";
		ss << "Expected:" << t1;
		ss << ", Got:" << t2;
		std::cout << ss.str();
		throw std::runtime_error(ss.str());
	}
}

template <typename T, typename F>
auto time(F f)
{
	auto start = std::chrono::steady_clock::now();

	auto result = f();

	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	return std::make_pair(std::chrono::duration<double, T>(diff).count(), result);
}

template <typename F>
auto test(const std::string& name, F f, double showDuration = 0.5, std::ostream& os = std::cout)
{
	os << "Running test '" << name << "' ";
	os.flush();

	try
	{
		auto result = time<std::micro>(f);
		long duration = static_cast<long>(result.first);


		os << "[" << duration << " us] ";
		os << "-> Success" << std::endl;
		return result.second;
	}
	catch (const std::exception& e)
	{
		os << "-> Failed !" << std::endl;
		os << "\t => " << e.what() << std::endl;
		return decltype(f()){};
	}
}

struct heavy {
	int map[1024];

	heavy() {
		for (auto& x : map)
			x = (int)(std::rand() * 10.0 / RAND_MAX);
	}
};

struct light {
	int map[1];

	light() {
		for (auto& x : map)
			x = (int)(std::rand() * 10.0 / RAND_MAX);
	}
};

template <typename T>
void bench()
{
	std::vector<T> data(200000);
	auto enumarable = LINQ(data);
	auto x1 = test("take-vector", [&]() {
		int sum = 0;
		int i = 0;
		for (int i = 0; i < 10000; i++) {
			sum += data[i].map[0];
		}
		return sum;
	});

	auto x2 = test("take-enumerable", [&]() {
		return enumarable
			.Take(10000)
			.template Select<int>([](const auto& x) { return x.map[0]; })
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
		return enumarable
			.template Select<int>([](const auto& x) { return x.map[0]; })
			.Where([](const auto& x) { return x > 5; })
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
		return enumarable
			.Where([](const auto& x) { return x.map[0] > 5; })
			.Take(1)
			.template Select<int>([](const auto& x) { return x.map[0]; })
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
		return enumarable
			.SkipWhile([](const auto& x) { return x.map[0] < 5; })
			.TakeWhile([](const auto& x) { return x.map[0] >= 5; })
			.template Select<int>([](const auto& x) { return x.map[0]; })
			.Sum();
	});
	
	assertEquals(x1, x2);
}

int main(int argc, char **argv)
{
	std::cout << "# Light objects" << std::endl;
	bench<light>();

	std::cout << "# Heavy objects" << std::endl;
	bench<heavy>();

	return EXIT_SUCCESS;
}