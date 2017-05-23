#pragma once

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

	DefineOrderBy(T, Iterator, Type::ClonedContainer, NullType);
};


template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::ClonedContainer>
{
	typename std::vector<T>::const_iterator current;
	typename std::vector<T>::const_iterator last;
	std::vector<T> *copiedContainer;
public:


	IEnumerable<T, Iterator, Type::ClonedContainer>(const IEnumerable<T, Iterator, Type::ClonedContainer>& other) : copiedContainer(other.copiedContainer),
		current(other.current),
		last(other.last)
	{
	}

	IEnumerable<T, Iterator, Type::ClonedContainer>& operator=(const IEnumerable<T, Iterator, Type::ClonedContainer>& other )
	{
		copiedContainer = new std::vector<T>( *other.copiedContainer );
		current = copiedContainer->begin() + (other.current - other.copiedContainer->begin());
		last = copiedContainer->end();
	}

	IEnumerable(Iterator ite, Iterator last)
	{
		copiedContainer = new std::vector<T>();
		for (; ite != last; ++ite)
			copiedContainer->push_back( *ite );
		current = copiedContainer->begin();
		this->last = copiedContainer->end();
	}

	~IEnumerable<T, Iterator, Type::ClonedContainer>()
	{
		delete copiedContainer;
	}

	inline void operator++() noexcept
	{
		++current;
	}

	inline const T &operator*() noexcept
	{
		return *current;
	}

	const IEnumerable<T, Iterator, Type::ClonedContainer> end()
	{
		return IEnumerable<T, Iterator, Type::ClonedContainer>(last, last);
	}

	//common definations
	DefineBegin();
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