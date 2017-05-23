#pragma once
#include <vector>
#include <algorithm>
#include <unittest++/UnitTest++.h>
#include "IEnumerable.h"

struct TestData {
	int f1;
	float f2;

	TestData() {
		f1 = (int)(std::rand() * 100.0 / RAND_MAX);
		f2 = (float)(std::rand()/ std::rand());
	}
};

template <typename T = TestData>
class Creator
{
	std::vector<TestData> vec;
	Creator()
	{
		vec.resize(2000000);
	}
	
	Creator( const Creator& ) = delete;
	Creator &operator=(const Creator&) = delete;
public:
	
	static const Creator& GetInstance()
	{
		static Creator c;
		return c;
	}
	
	auto GetData() const
	{
		auto linq = LINQ(vec);
		return linq;
	}
	
	const std::vector<T> &GetVector() const
	{
		return vec;
	}
	
	auto GetSize() const
	{
		return vec.size();
	}
	
	~Creator() = default;
};
