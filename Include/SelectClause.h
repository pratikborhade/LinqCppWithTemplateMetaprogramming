#pragma once

template<
	typename T,
	typename Iterator,
	typename Functor
>
class IEnumerable<T, Iterator, Type::Select, Functor>
{
public:
	struct SelectIterator
	{
		Iterator ite;
		Iterator last;
		Functor const functor;
		typedef typename Iterator::value_type value_type;

		SelectIterator() = default;

		SelectIterator(Iterator &ite, Iterator &last, Functor const &func) : ite(ite), last(last), functor(func)
		{
		}

		SelectIterator(const SelectIterator&) = default;

		SelectIterator& operator=(const SelectIterator& other)
		{
			ite = other.ite;
		}

		inline void operator++() noexcept
		{
			++ite;
		}

		inline decltype(auto) operator*() const noexcept
		{
			return functor(*ite);
		}

		inline bool operator==(const SelectIterator& other) const noexcept
		{
			return ite == other.ite;
		}

		inline bool operator!=(const SelectIterator& other) const noexcept
		{
			return !(ite == other.ite);
		}

		auto end()
		{
			static SelectIterator end(last, last, functor);
			return end;
		}
	};
private:
	SelectIterator current;
public:

	IEnumerable(Iterator &ite, Iterator &last, Functor const & func) :
		current(ite, last, func)
	{
	}

	inline void operator++() noexcept
	{
		++current;
	}

	inline decltype(auto) operator*() noexcept
	{
		return *current;
	}


	const IEnumerable<T, Iterator, Type::Select> end() noexcept
	{
		return IEnumerable<T, Iterator, Type::Select>(current.last, current.last, {});
	}

	//common definations
	DefineBegin();
	DefineWhere(T, SelectIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Select);
	DefineTake(T, SelectIterator, current, current.end());
	DefineTakeWhile(T, SelectIterator, current, current.end());

	DefineSkipMethods(T, SelectIterator, *this);

	DefineSelect(T, SelectIterator, current, current.end());
	DefineSum(T, SelectIterator, current, current.end());

	DefineOrderBy(T, Iterator, Type::Select, Functor);
};

template<
	typename T,
	typename Iterator
>
template<typename Func>
auto IEnumerable<T, Iterator, Type::None>::Select(Iterator begin, Iterator last, Func const & func)
{
	return IEnumerable<
		typename std::remove_cv< typename std::remove_reference<typename std::result_of< Func(T)>::type>::type >::type,
		Iterator,
		Type::Select, 
		Func>(begin, last, func);
}


template<
	typename T,
	typename Iterator
>
template <typename Q, typename E>
auto IEnumerable<T, Iterator, Type::None>::Select(Iterator begin, Iterator last, Q E::* field)
{
	auto lambda = [field](const E& val)-> const Q & 
		{
			return val.*field;
		};
	return IEnumerable<Q, Iterator, Type::Select, decltype(lambda)>(begin, last, lambda);
}

