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

template<bool, typename T>
struct CustomEnableIf
{};


template<typename T>
struct CustomEnableIf<true, T>
{
	typedef T type;
};


template<typename T>
struct CustomEnableIf<false, T>
{
	typedef NullType type;	
};

template<
	typename T,
	typename Iterator,
	Type type
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
	static auto Where(Iterator begin, Iterator last, std::function< bool(const T&) > const &functor);

	static auto Take( Iterator begin, Iterator last, std::size_t limit);

	static auto TakeWhile(Iterator begin, Iterator last, std::function< bool(const T&) > const &functor);

	template< typename BaseClass >
	static auto SkipWhile(BaseClass begin, std::function< bool(const T&) > const &functor)
	{
		auto end = begin.end();
		while (begin != end && functor(*begin))
			++begin;
		return begin;
	}

	template< typename BaseClass >
	static auto Skip(BaseClass begin, const size_t limit)
	{
		auto end = begin.end();
		auto l = limit;
		while (begin != end && l > 1)
		{
			--l;
			++begin;
		}
		return begin;
	}

	static T Sum(Iterator begin, Iterator last)
	{
		T sum = 0;

		for (; begin != last; ++begin)
			sum += T(*begin);
		return sum;
	}

	template< typename Container = std::vector<T> >
	static Container ToList(Iterator begin, Iterator last)
	{
		Container container;
		for ( ; begin != last; ++ begin )
			container.push_back(*begin);
		return container;
	}

	template <typename Q>
	static auto Select(Iterator begin, Iterator last, std::function<const Q&(const T&)> const &functor);


	template <typename Q, typename E = typename CustomEnableIf<std::is_class<T>::value, T>::type>
	static auto Select(Iterator begin, Iterator last, Q E::* field);

};

#define DefineWhere( T, Iterator, begin, last )\
auto Where( std::function< bool(const T&) > const &functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Where(begin, last, functor);\
}

#define DefineTake( T, Iterator, begin, last )\
auto Take( size_t limit ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Take(begin, last, limit);\
}

#define DefineTakeWhile( T, Iterator, begin, last )\
auto TakeWhile( std::function< bool(const T&) > const &functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::TakeWhile(begin, last, functor);\
}

#define DefineSkipMethods( T, Iterator, begin )\
auto Skip( size_t limit ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::Skip(begin, limit);\
}\
auto SkipWhile( std::function< bool(const T&) > const &functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::SkipWhile(begin, functor);\
}

#define DefineBegin()\
auto begin()\
{ return *this; }

#define DefineBasicOperator(T, Iterator, Type)\
template<typename EqualClass> bool inline operator==( const EqualClass &other ) const { return current == other.current; };\
template<typename EqualClass> bool inline operator!=( const EqualClass &other ) const { return current != other.current; };

#define DefineSelect( T, Iterator, begin, last )\
template<typename Q> \
auto Select(std::function<const Q&(const T&)> const &functor)\
{\
	return IEnumerable<T, Iterator, Type::None>::template Select<Q>( begin, last, functor );\
}\
template <typename Q, typename E = typename CustomEnableIf<std::is_class<T>::value, T>::type>\
auto Select(Q E::* field)\
{\
	return IEnumerable<T, Iterator, Type::None>::template Select<Q>( begin, last, field );\
}


#define DefineSum( T, Iterator, begin, last )\
T Sum()\
{\
	return IEnumerable<T, Iterator, Type::None>::Sum(begin, last);\
}\
template <typename ListContainer = std::vector<T>>\
ListContainer ToList()\
{ return IEnumerable<T, Iterator, Type::None>::template ToList<ListContainer>(begin, last); }

#include "WhereClause.h"
#include "LinqConvertor.h"
#include "TakeClause.h"
#include "SelectClause.h"