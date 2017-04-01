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
		Functor functor;
		size_t limit;
		TakeType type;

		TakeIterator() = default;

		TakeIterator(Iterator &ite, Iterator &last, Functor &func) : ite(ite), last(last), functor(func), type(TakeType::Function), limit(0)
		{
		}


		TakeIterator(Iterator &ite, Iterator &last, const size_t &limit) : ite(ite), last(last), limit(limit), type(TakeType::Limit), functor({})
		{
		}

		TakeIterator(const TakeIterator&) = default;

		TakeIterator& operator=(const TakeIterator&) = default;

		void operator++()
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
				if (limit <= 1)
					ite = last;
			default:
				break;
			}
		}

		const T operator*() const
		{
			return *ite;
		}

		bool operator==(const TakeIterator& other) const
		{
			return ite == other.ite;
		}

		bool operator!=(const TakeIterator& other) const
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
	Functor functor;
	size_t limit;
public:

	IEnumerable(Iterator &ite, Iterator &last, Functor &func) :
		functor(func),
		current(ite, last, func)
	{
	}

	IEnumerable(Iterator &ite, Iterator &last, const size_t &limit) :
		limit(limit),
		current(ite, last, limit)
	{
	}

	void operator++()
	{
		++current;
	}

	const T operator*()
	{
		return *current;
	}


	auto end()
	{
		return IEnumerable<T, Iterator, Type::Take>(current.last, current.last, functor);
	}

	//common definations
	DefineBegin();
	DefineWhere(T, TakeIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Take);

	DefineTake(T, TakeIterator, current, current.end());
	DefineTakeWhile(T, TakeIterator, current, current.end());


	DefineSkipMethods(T, TakeIterator, current, current.end());

	DefineSelect(T, TakeIterator, current, current.end());
	DefineSum(T, TakeIterator, current, current.end())
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
auto IEnumerable<T, Iterator, Type::None>::TakeWhile(Iterator begin, Iterator last, std::function<bool(const T&)> func)
{
	return IEnumerable<T, Iterator, Type::Take>(begin, last, func);
}