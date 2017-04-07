#pragma once

template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::Where>
{
	using Functor = std::function<bool(const T&)>;
public:
	struct WhereIterator
	{
		Iterator ite;
		Iterator last;
		Functor const functor;
		
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

		inline const T &operator*() const noexcept
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
			static WhereIterator end(last, last, {});
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

	inline const T &operator*() noexcept
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
	DefineSum(T, WhereIterator, current, current.end())
};

template<
	typename T,
	typename Iterator
>
auto IEnumerable<T, Iterator, Type::None>::Where(Iterator begin, Iterator last, std::function<bool(const T&)> const &func)
{
	return IEnumerable<T, Iterator, Type::Where>(begin, last, func);
}

