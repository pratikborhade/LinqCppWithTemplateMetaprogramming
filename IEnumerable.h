#pragma once

#include <functional>
#include <type_traits>

enum Type
{
	None,
	Container,
	Where,
	Take,
	Skip,
	Select
};

struct NullType
{};

template<
	typename T,
	typename Iterator,
	Type type,
	typename Q = NullType //optional
>
class IEnumerable
{
};

template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::None>
{
public:
	static auto Where(Iterator begin, Iterator last, std::function< bool(const T&) > functor);

	static auto Take( Iterator begin, Iterator last, std::size_t limit);

	static auto TakeWhile(Iterator begin, Iterator last, std::function< bool(const T&) > functor);

	static auto SkipWhile(Iterator begin, Iterator last, std::function< bool(const T&) > functor)
	{
		Iterator ite = begin;
		while (ite != last && functor(*ite))
			++ite;
		return ite;
	}

	static auto Skip(Iterator begin, Iterator last, const size_t limit)
	{
		Iterator ite = begin();
		auto l = limit;
		while (ite != last && l > 1)
		{
			--l;
			++ite;
		}
		return ite;
	}

	static T Sum(Iterator begin, Iterator last)
	{
		T sum = 0;

		for (; begin != last; ++begin)
			sum += *begin;
		return sum;
	}

	template <typename Q>
	static auto Select(Iterator begin, Iterator last, std::function<Q(const T&)> functor);
};

#define DefineWhere( T, Iterator, begin, last )\
auto Where( std::function< bool(const T&) > functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Where(begin, last, functor);\
}

#define DefineTake( T, Iterator, begin, last )\
auto Take( size_t limit ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Take(begin, last, limit);\
}

#define DefineTakeWhile( T, Iterator, begin, last )\
auto TakeWhile( std::function< bool(const T&) > functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::TakeWhile(begin, last, functor);\
}

#define DefineSkipMethods( T, Iterator, begin, last )\
auto Skip( size_t limit ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Skip(begin, last, limit);\
}\
auto SkipWhile( std::function< bool(const T&) > functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::SkipWhile(begin, last, functor);\
}

#define DefineBegin()\
auto begin()\
{ return *this; }

#define DefineBasicOperator(T, Iterator, Type)\
template<typename EqualClass> bool operator==( const EqualClass &other ){ return current == other.current; };\
template<typename EqualClass> bool operator!=( const EqualClass &other ){ return current != other.current; };

#define DefineSelect( T, Iterator, begin, last )\
template<typename Q> \
auto Select(std::function<Q(const T&)> functor)\
{\
	return IEnumerable<T, Iterator, Type::None>::Select<Q>( begin, last, functor );\
}

#define DefineSum( T, Iterator, begin, last )\
T Sum()\
{\
	return IEnumerable<T, Iterator, Type::None>::Sum(begin, last);\
}

#include "WhereClause.h"
#include "LinqConvertor.h"
#include "TakeClause.h"
#include "SelectClause.h"