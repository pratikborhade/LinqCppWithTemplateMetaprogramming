#pragma once


template<
	typename T,
	typename Iterator
>
class IEnumerable<T, Iterator, Type::Take>
{
	using Functor = std::function<bool(const T&)>;
	enum TakeType
	{
		Function,
		Limit
	};

	struct TakeIterator
	{
		Iterator ite;
		Iterator last;
		Functor const functor;
		size_t limit;
		TakeType type;


		typedef typename Iterator::value_type value_type;

		TakeIterator() = default;

		TakeIterator(Iterator &ite, Iterator &last, Functor const &func) : ite(ite), last(last), functor(func), type(TakeType::Function), limit(0)
		{
		}


		TakeIterator(Iterator &ite, Iterator &last, const size_t &limit) : ite(ite), last(last), limit(limit), type(TakeType::Limit), functor({})
		{
		}

		TakeIterator(const TakeIterator&) = default;

		TakeIterator& operator=(const TakeIterator& other)
		{
			ite = other.ite;
			limit = other.limit;
		}

		inline void operator++() noexcept
		{
			++ite;
			switch (type)
			{
			case TakeType::Function:
				if (!functor(*ite))
					ite = last;
				break;
			case TakeType::Limit:
				--limit;
				if (limit == 0)
					ite = last;
			default:
				break;
			}
		}

		inline const T &operator*() const noexcept
		{
			return *ite;
		}

		inline bool operator==(const TakeIterator& other) const noexcept
		{
			return ite == other.ite;
		}

		inline bool operator!=(const TakeIterator& other) const noexcept
		{
			return !(ite == other.ite);
		}

		auto end()
		{
			static TakeIterator end(last, last, 0);
			return end;
		}
	};

private:
	TakeIterator current;
	size_t limit;
public:

	IEnumerable(Iterator &ite, Iterator &last, Functor const &func) :
		current(ite, last, func)
	{
	}

	IEnumerable(Iterator &ite, Iterator &last, const size_t &limit) :
		limit(limit),
		current(ite, last, limit)
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


	auto end()
	{
		return IEnumerable<T, Iterator, Type::Take>(current.last, current.last, {});
	}

	//common definations
	DefineBegin();
	DefineWhere(T, TakeIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Take);

	DefineTake(T, TakeIterator, current, current.end());
	DefineTakeWhile(T, TakeIterator, current, current.end());


	DefineSkipMethods(T, TakeIterator, *this);

	DefineSelect(T, TakeIterator, current, current.end());
	DefineSum(T, TakeIterator, current, current.end());

	DefineOrderBy(T, Iterator, Type::Take, NullType);
};


template<
	typename T,
	typename Iterator
>
auto IEnumerable<T, Iterator, Type::None>::Take(Iterator begin, Iterator last, std::size_t limit)
{
	return IEnumerable<T, Iterator, Type::Take>(begin, last, limit);
}

template<
	typename T,
	typename Iterator
>
auto IEnumerable<T, Iterator, Type::None>::TakeWhile(Iterator begin, Iterator last, std::function<bool(const T&)> const &func)
{
	return IEnumerable<T, Iterator, Type::Take>(begin, last, func);
}