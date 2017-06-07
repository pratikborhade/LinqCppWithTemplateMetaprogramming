#pragma once
#include <memory>

template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::Container>
{
	Iterator current;
	Iterator last;
public:
	IEnumerable(Iterator ite, Iterator last) : last(last), current(ite)
	{
	}

	inline void operator++() noexcept
	{
		++current;
	}

	inline const T& operator*() noexcept
	{
		return *current;
	}

	const IEnumerable<T, Iterator, Type::Container> end()
	{
		return IEnumerable<T, Iterator, Type::Container>(last, last);
	}

	//common definations
	DefineBegin();
	DefineWhere(T, Iterator, current, last);
	DefineBasicOperator(T, Iterator, Type::Container);

	DefineTake(T, Iterator, current, last);
	DefineTakeWhile(T, Iterator, current, last);

	DefineSkipMethods(T, Iterator, *this);

	DefineSelect(T, Iterator, current, last);
	DefineSum(T, Iterator, current, last);

	DefineOrderBy(T, Iterator, Type::Container, NullType);
};


template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::ClonedContainer>
{
	using Container = std::vector<T>;
	typename Container::const_iterator current;
	typename Container::const_iterator last;
	std::shared_ptr<Container> copiedContainer;
	
	IEnumerable<T, Iterator, Type::ClonedContainer>(
		const std::shared_ptr<Container> &copiedContainer,
		typename Container::const_iterator &current,
		typename Container::const_iterator &last ):copiedContainer(copiedContainer), last(last), current(current)
	{
	}
	
public:

	IEnumerable<T, Iterator, Type::ClonedContainer>(const IEnumerable<T, Iterator, Type::ClonedContainer>& other):
		copiedContainer(other.copiedContainer),
		current(other.current),
		last(other.last)
	{
	}
	
	IEnumerable<T, Iterator, Type::ClonedContainer>& operator=(const IEnumerable<T, Iterator, Type::ClonedContainer>& other )
	{
		copiedContainer = other.copiedContainer;
		current = other.current;
		last = other.last;
	}

	IEnumerable(Iterator ite, Iterator lastIte):
		copiedContainer(new Container())
	{
		for (; ite != lastIte; ++ite)
			copiedContainer->push_back( *ite );
		current = copiedContainer->begin();
		last = copiedContainer->end();
	}
	
	IEnumerable<T, Iterator, Type::ClonedContainer>(Container&& vec):
		copiedContainer(new Container( vec ))
	{
		current = copiedContainer->begin();
		last = copiedContainer->end();
	}
	
	inline void operator++() noexcept
	{
		++current;
	}

	inline const T &operator*() noexcept
	{
		return *current;
	}

	decltype(auto) end()
	{
		return IEnumerable<T, Iterator, Type::ClonedContainer>(copiedContainer, last, last);
	}
	
	decltype(auto) begin()
	{
		return *this;
	}
	
	//common definations
	DefineWhere(T, Iterator, current, last);
	DefineBasicOperator(T, Iterator, Type::ClonedContainer);

	DefineTake(T, Iterator, current, last);
	DefineTakeWhile(T, Iterator, current, last);

	DefineSkipMethods(T, Iterator, *this);

	DefineSelect(T, Iterator, current, last);
	DefineSum(T, Iterator, current, last);


	DefineOrderBy(T, Iterator, Type::ClonedContainer, NullType);
};


template <typename Container, typename T = typename std::iterator_traits<typename Container::iterator>::value_type>
auto LINQ(const Container &container)
{
	return  IEnumerable<T, typename Container::const_iterator, Type::Container>(container.begin(), container.end());
}

template <typename Container, typename T = typename std::iterator_traits<typename Container::iterator>::value_type>
auto LINQCOPY(const Container &container)
{
	return  IEnumerable<T, typename Container::const_iterator, Type::ClonedContainer>(container.begin(), container.end());
}

template <typename Container, typename T = typename std::iterator_traits<typename Container::iterator>::value_type>
auto LINQCOPY(Container &&container)
{
	return  IEnumerable<T, typename Container::const_iterator, Type::ClonedContainer>(container);
}