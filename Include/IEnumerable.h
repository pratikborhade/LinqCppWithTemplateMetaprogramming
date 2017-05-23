#pragma once

#include <functional>
#include <type_traits>
#include <vector>
#include <algorithm>
enum Type
{
	None,
	Container,
	Where,
	Take,
	Skip,
	Select,
	ClonedContainer
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
	Type type,
	typename Functor = NullType
>
class IEnumerable
{
};

enum OrderByType
{
	asc,
	desc
};

template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::None>
{
public:
	template <typename Functor>
	static auto Where(Iterator begin, Iterator last, Functor const &functor);

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

	template < typename BaseType = typename std::remove_cv< typename std::remove_reference<T>::type >::type >
	static BaseType Sum(Iterator begin, Iterator last)
	{
		BaseType sum = *begin;
		++begin;
		return std::accumulate( begin, last, sum );
	}

	template<typename BaseType = typename std::remove_cv< typename std::remove_reference<T>::type >::type, typename Container = std::vector<BaseType> >
	static Container ToList(Iterator begin, Iterator last)
	{
		Container container;
		for ( ; begin != last; ++ begin )
			container.push_back(*begin);
		return container;
	}

	template <typename Functor>
	static auto Select(Iterator begin, Iterator last, Functor const &functor);


	template <typename Q, typename E = typename CustomEnableIf<std::is_class<T>::value, T>::type>
	static auto Select(Iterator begin, Iterator last, Q E::* field);

	template <Type type, typename  anotherType1, typename ... Args >
	static auto OrderBy(IEnumerable<T, Iterator, type, anotherType1> &container,Args &&... args );
};

#define DefineWhere( T, Iterator, begin, last )\
template< typename Func >\
auto Where( Func const &functor ) \
{ \
	return IEnumerable<T, Iterator, Type::None>::template Where<Func>(begin, last, functor);\
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
template<typename Func> \
auto Select(Func const &functor)\
{\
	return IEnumerable<T, Iterator, Type::None>::template Select<Func>( begin, last, functor );\
}\
template <typename Q, typename E = typename CustomEnableIf<std::is_class<T>::value, T>::type>\
auto Select(Q E::* field)\
{\
	return IEnumerable<T, Iterator, Type::None>::template Select( begin, last, field );\
}

#define DefineOrderBy( T, Iterator, type, AnotherType )\
template< typename ... Args >\
auto OrderBy(Args &&... args)\
{\
	return IEnumerable< T, Iterator, Type::None >::template OrderBy<type, AnotherType, Args ...>(*this, std::forward<Args>(args) ...);\
}


#define DefineSum( T, Iterator, begin, last )\
template <typename BaseType = typename std::remove_cv< typename std::remove_reference<T>::type >::type>\
BaseType Sum()\
{\
	return IEnumerable<T, Iterator, Type::None>::Sum(begin, last);\
}\
auto ToList()\
{ return IEnumerable<T, Iterator, Type::None>::ToList(begin, last); }

#include "WhereClause.h"
#include "LinqConvertor.h"
#include "TakeClause.h"
#include "SelectClause.h"
#include "OrderByClause.h"