#pragma once

template<
	typename T,
	typename Iterator,
	typename Functor
>
class IEnumerable<T, Iterator, Type::Where, Functor>
{
private:

	struct WhereIterator
	{
		Iterator ite;
		Iterator last;
		Functor const functor;
	public:
		typedef typename Iterator::value_type value_type;

		WhereIterator() = default;

		WhereIterator(Iterator &iterator, Iterator &last, Functor const &func) : ite(iterator), last(last), functor(func)
		{
			while (ite != last && !functor(*ite))
			{
				++ite;
			}
		}

		WhereIterator(const WhereIterator&) = default;

		WhereIterator& operator=(const WhereIterator& other)
		{
			ite = other.ite;
			return *this;
		}

		inline void operator++() noexcept
		{
			++ite;
			for (;ite != last && !functor(*ite); ++ite)
			{
			}
		}

		inline decltype(auto) operator*() const noexcept
		{
			return *ite;
		}

		inline bool operator==(const WhereIterator& other) const noexcept
		{
			return ite == other.ite;
		}

		inline bool operator!=(const WhereIterator& other) const noexcept
		{
			return !(ite == other.ite);
		}

		auto end()
		{
			static WhereIterator end(last, last, functor);
			return end;
		}
	};
private:
	WhereIterator current;
public:

	IEnumerable(Iterator &ite, Iterator &last, Functor const &func) :
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


	const IEnumerable<T, Iterator, Type::Where> end()
	{
		return IEnumerable<T, Iterator, Type::Where>(current.last, current.last, {});
	}

	//common definations
	DefineBegin();
	DefineWhere(T, WhereIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Where);
	DefineTake(T, WhereIterator, current, current.end());
	DefineTakeWhile( T, WhereIterator, current, current.end() );

	DefineSkipMethods(T, WhereIterator, *this);

	DefineSelect(T, WhereIterator, current, current.end());
	DefineSum(T, WhereIterator, current, current.end());

	DefineOrderBy( T, Iterator, Type::Where, Functor  );
};

template<
	typename T,
	typename Iterator
>
template <typename Functor>
auto IEnumerable<T, Iterator, Type::None>::Where(Iterator begin, Iterator last, Functor const &func)
{
	return IEnumerable<T, Iterator, Type::Where, Functor>(begin, last, func);
}

