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

	inline const T &operator*() noexcept
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
	DefineSum(T, Iterator, current, last)
};

template <typename Container, typename T = typename std::iterator_traits<typename Container::iterator>::value_type>
auto LINQ(const Container &container)
{
	return  IEnumerable<T, typename Container::const_iterator, Type::Container>(container.begin(), container.end());
}