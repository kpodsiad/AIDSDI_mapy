#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>
#include <vector>

#define DEF_CAPACITY 1024

namespace aisdi
{

template<typename KeyType, typename ValueType>
class HashMap
{
public:
	using key_type = KeyType;
	using mapped_type = ValueType;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using reference = value_type &;
	using const_reference = const value_type &;
	using listIterator = typename std::list<value_type>::iterator;

	class ConstIterator;

	class Iterator;

	using iterator = Iterator;
	using const_iterator = ConstIterator;


	size_t HASH = DEF_CAPACITY;
	size_t buckets = DEF_CAPACITY;
	size_t addedElements = 0;
	std::vector< std::list<value_type> >table;


	void insert(value_type value)
	{
		size_t index = value.first % HASH;;
		table[index].push_back(value);
		++addedElements;
	}

	size_t lastHashEntry() const
	{
		size_t lastEntry = buckets;

		for (size_t i = buckets-1; i > 0 ; --i)
		{
			if(table[i].size() != 0)
			{
				lastEntry = i;
				break;
			}
		}
		return lastEntry;
	}

	size_t firstHashEntry() const
	{
		size_t lastEntry = buckets;

		for (size_t i = 0; i < buckets ; ++i)
		{
			if(table[i].size() != 0)
			{
				lastEntry = i;
				break;
			}
		}
		return lastEntry;
	}

public:

	HashMap()
	{
		table.resize(buckets);
	}

	HashMap(size_t HASH, size_t buckets):HASH(HASH), buckets(buckets)
	{
		table.resize(buckets);
	}

	HashMap(std::initializer_list<value_type> list) : HashMap()
	{
		for(auto elem : list)
			insert(elem);
	}

	HashMap(const HashMap &other) :HashMap(other.HASH, other.buckets)
	{
		for(auto elem : other)
			insert(elem);
	}

	HashMap(HashMap &&other): HASH(other.HASH), buckets(other.buckets), addedElements(other.addedElements)
	{
		table = std::move(other.table);
	}

	HashMap &operator=(const HashMap &other)
	{
		if(this != &other)
		{
			table.clear();
			HASH = other.HASH;
			buckets = other.buckets;
			table.resize(buckets);
			addedElements = 0;

			for(auto elem : other)
				insert(elem);
		}
		return *this;
	}

	HashMap &operator=(HashMap &&other)
	{
		if(this != &other)
		{
			table.clear();
			HASH = other.HASH;
			buckets = other.buckets;
			addedElements = other.addedElements;
			table = std::move(other.table);
		}
		return *this;
	}

	bool isEmpty() const
	{
		return addedElements==0;
	}

	mapped_type &operator[](const key_type &key)
	{
		auto it = find(key);
		if(it == cend())
		{
			insert({key, mapped_type{}});
			return table[key % HASH].back().second;
		}
		else
			return it->second;
	}

	const mapped_type &valueOf(const key_type &key) const
	{
		auto it = find(key);

		if(it == end())
			throw std::out_of_range("key doesn't exist");

		return it->second;
	}

	mapped_type &valueOf(const key_type &key)
	{
		return const_cast<mapped_type&>(static_cast<const HashMap>(*this).valueOf(key));
	}

	const_iterator find(const key_type &key) const
	{
		size_t index = key % HASH;
		int i = 0;
		for(auto elem : table[index])
		{
			if(elem.first == key)
				return ConstIterator(this, index, i);
			++i;
		}
		return cend();
	}

	iterator find(const key_type &key)
	{
		return Iterator( (static_cast<const HashMap>(*this)).find(key) );
	}

	void remove(const key_type &key)
	{
//		size_t index = key % HASH;
//
//		if(table[index].size() == 0)
//			throw std::out_of_range("Key not found");
//
//		auto it = find(key);
//		if(it == end())
//			throw std::out_of_range("Key not found");
//
//		table[it.hashIndex].erase(table[it.hashIndex].begin() + it.innerIndex);
//		--addedElements;
		(void)key;
		throw std::runtime_error("TODO");
	}

	void remove(const const_iterator &it)
	{
//		remove(it->first);
		(void)it;
		throw std::runtime_error("TODO");
	}

	size_type getSize() const
	{
		return addedElements;
	}

	bool operator==(const HashMap &other) const
	{
		auto it = begin();

		for(auto elem : other)
		{
			if(*it != elem)
			{
				return false;
			}
		}
		return true;
	}

	bool operator!=(const HashMap &other) const
	{
		return !(*this == other);
	}

	iterator begin()
	{
		throw Iterator(cbegin());
	}

	iterator end()
	{
		throw Iterator(cend());
	}

	const_iterator cbegin() const
	{
		if(addedElements == 0)
			return cend();

		size_t firstEntry = buckets;
		for (size_t i = 0; i < buckets ; ++i)
		{
			if(table[i].size() != 0)
			{
				firstEntry = i;
				return ConstIterator(this, firstEntry, table[firstEntry].begin());
			}
		}
	}

	const_iterator cend() const
	{
		return ConstIterator(this, buckets);
	}

	const_iterator begin() const
	{
		return cbegin();
	}

	const_iterator end() const
	{
		return cend();
	}
};

template<typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename HashMap::const_reference;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename HashMap::value_type;
	using pointer = const typename HashMap::value_type *;
	using listIterator = HashMap::listIterator;

	friend class HashMap;

private:
	const HashMap *map = nullptr;
	size_t hashIndex = 0;
	listIterator currentIterator;

	std::list<value_type> &getTable() const
	{ return map->table[hashIndex]; }

public:

	explicit ConstIterator() = default;

	ConstIterator(const HashMap *map, size_t hashIndex, listIterator currentIterator={})
			: map(map), hashIndex(hashIndex), currentIterator(currentIterator)
	{
	}
		ConstIterator(const ConstIterator &other)
	: map(other.map), hashIndex(other.hashIndex), currentIterator(other.currentIterator)
	{
	}

	ConstIterator &operator++()
	{
		if(map == nullptr)
			throw std::logic_error("collection not given to iterator");

		if(hashIndex == map->buckets)
			throw std::out_of_range("iterator out of range");

		++currentIterator;
		while(currentIterator == getTable().end())
		{
			++hashIndex;

			if(hashIndex == map->buckets)
			{
				currentIterator = map->end();
				return *this;
			}

			currentIterator = getTable().begin();
		}

		return *this;
	}

	ConstIterator operator++(int)
	{
		auto it = *this;
		this->operator++();
		return it;
	}

	ConstIterator &operator--()
	{
		if(map == nullptr)
			throw std::logic_error("collection not given to iterator");

		if(*this == map->begin())
			throw std::out_of_range("iterator out of range");

		if(hashIndex == map->buckets)
		{
			--hashIndex;
			currentIterator = getTable().end();
		}

		while(currentIterator == getTable().end())
		{

		}

		return *this;
	}

	ConstIterator operator--(int)
	{
		auto it = *this;
		this->operator--();
		return it;
	}

	reference operator*() const
	{
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	bool operator==(const ConstIterator &other) const
	{

	}

	bool operator!=(const ConstIterator &other) const
	{
		return !(*this == other);
	}
};


	template<typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename HashMap::reference;
	using pointer = typename HashMap::value_type *;

	explicit Iterator()
	{}

	Iterator(const ConstIterator &other)
			: ConstIterator(other)
	{}

	Iterator &operator++()
	{
		ConstIterator::operator++();
		return *this;
	}

	Iterator operator++(int)
	{
		auto result = *this;
		ConstIterator::operator++();
		return result;
	}

	Iterator &operator--()
	{
		ConstIterator::operator--();
		return *this;
	}

	Iterator operator--(int)
	{
		auto result = *this;
		ConstIterator::operator--();
		return result;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	reference operator*() const
	{
		// ugly cast, yet reduces code duplication.
		return const_cast<reference>(ConstIterator::operator*());
	}
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
