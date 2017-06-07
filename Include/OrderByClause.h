#pragma once
#include <algorithm>

template <typename T>
bool CheckFunctor(const T&, const T&)
{
	return false;
}

template <typename T, typename Q, typename ... Args >
bool CheckFunctor(const T& value1, const T& value2, Q T::* field, OrderByType orderBy = OrderByType::asc, Args ... args)
{
	if (value1.*field < value2.*field)
		return orderBy == OrderByType::asc;
	else if (value1.*field > value2.*field)
		return orderBy == OrderByType::desc;
	return CheckFunctor(value1, value2, args ... );
}

template <typename T, typename Q, typename ... Args >
bool CheckFunctor(const T& value1, const T& value2, Q T::* field, Args ... args)
{
	if (value1.*field < value2.*field)
		return true;
	else if (value1.*field > value2.*field)
		return false;
	return CheckFunctor(value1, value2, args ... );
}

template<
	typename T,
	typename Iterator
>
template < typename Container, typename ... Args >
auto  IEnumerable<T, Iterator, Type::None>::OrderBy(Container &container, Args &&... args)
{
	auto lambda = [&](const T& v1, const T& v2) -> bool
	{
		return CheckFunctor( v1, v2, std::forward<Args>(args)... );
	};

	auto list = container.ToList();
	std::sort(list.begin(), list.end(), lambda);
	using baseType = std::remove_cv_t<T>;
	return IEnumerable<baseType, typename std::vector<baseType>::const_iterator, Type::ClonedContainer>(std::move(list) );
}