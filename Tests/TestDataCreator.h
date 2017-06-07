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
		f2 = (float)(std::rand())/ float(std::rand());
	}
};

template <typename T = TestData, std::size_t size = 200000>
class Creator
{
	std::vector<TestData> vec;
	Creator()
	{
		vec.resize(size);
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
