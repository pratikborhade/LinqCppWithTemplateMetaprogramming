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
		Functor functor;

		WhereIterator() = default;

		WhereIterator(Iterator &ite, Iterator &last, Functor &func) : ite(ite), last(last), functor(func)
		{
		}

		WhereIterator(const WhereIterator&) = default;

		WhereIterator& operator=(const WhereIterator&) = default;

		void operator++()
		{
			++ite;
			while (ite != last && !functor(*ite))
			{
				++ite;
			}
		}

		const T operator*() const
		{
			return *ite;
		}

		bool operator==(const WhereIterator& other) const
		{
			return ite == other.ite;
		}

		bool operator!=(const WhereIterator& other) const
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
	Functor functor;
public:

	IEnumerable(Iterator &ite, Iterator &last, Functor &func) :
		functor(func),
		current(ite, last, func)
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


	const IEnumerable<T, Iterator, Type::Where> end()
	{
		return IEnumerable<T, Iterator, Type::Where>(current.last, current.last, functor);
	}

	//common definations
	DefineBegin();
	DefineWhere(T, WhereIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Where);
	DefineTake(T, WhereIterator, current, current.end());
	DefineTakeWhile( T, WhereIterator, current, current.end() );

	DefineSkipMethods(T, WhereIterator, current, current.end());

	DefineSelect(T, WhereIterator, current, current.end());
	DefineSum(T, WhereIterator, current, current.end())
};

template<
	typename T,
	typename Iterator
>
auto IEnumerable<T, Iterator, Type::None>::Where(Iterator begin, Iterator last, std::function<bool(const T&)> func)
{
	return IEnumerable<T, Iterator, Type::Where>(begin, last, func);
}

