#pragma once

template<
	typename T,
	typename Iterator,
	typename Inp
>
class IEnumerable<T, Iterator, Type::Select, Inp>
{
	using Functor = std::function<T(const Inp&)>;
public:
	struct SelectIterator
	{
		Iterator ite;
		Iterator last;
		Functor functor;

		SelectIterator() = default;

		SelectIterator(Iterator &ite, Iterator &last, Functor &func) : ite(ite), last(last), functor(func)
		{
		}

		SelectIterator(const SelectIterator&) = default;

		SelectIterator& operator=(const SelectIterator&) = default;

		void operator++()
		{
			++ite;
		}

		const T operator*() const
		{
			return functor(*ite);
		}

		bool operator==(const SelectIterator& other) const
		{
			return ite == other.ite;
		}

		bool operator!=(const SelectIterator& other) const
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


	const IEnumerable<T, Iterator, Type::Select> end()
	{
		return IEnumerable<T, Iterator, Type::Select>(current.last, current.last, functor);
	}

	//common definations
	DefineBegin();
	DefineWhere(T, SelectIterator, current, current.end());
	DefineBasicOperator(T, Iterator, Type::Select);
	DefineTake(T, SelectIterator, current, current.end());
	DefineTakeWhile(T, SelectIterator, current, current.end());

	DefineSkipMethods(T, SelectIterator, current, current.end());

	DefineSelect(T, SelectIterator, current, current.end());
	DefineSum(T, SelectIterator, current, current.end())
};

template<
	typename T,
	typename Iterator
>
template<typename Q>
auto IEnumerable<T, Iterator, Type::None>::Select(Iterator begin, Iterator last, std::function<Q(const T&)> func)
{
	return IEnumerable<Q, Iterator, Type::Select, T>(begin, last, func);
}
