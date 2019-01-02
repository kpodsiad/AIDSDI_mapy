#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>
#include <vector>

#define DEF_CAPACITY 1193

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
	using listIterator = typename std::list<value_type>::const_iterator;

	class ConstIterator;

	class Iterator;

	using iterator = Iterator;
	using const_iterator = ConstIterator;

	size_t buckets = DEF_CAPACITY;
	size_t addedElements = 0;
	std::vector< std::list<value_type> >table;


	void insert(value_type value)
	{
		size_t index = std::hash<key_type>{}(value.first) % buckets;
		table[index].push_back(value);
		++addedElements;
	}

	size_t myHash(const key_type &key) const
	{
		return std::hash<key_type >{}(key) % buckets;
	}

public:

	HashMap()
	{
		table.resize(buckets);
	}

	HashMap(size_t buckets) : buckets(buckets)
	{
		table.resize(buckets);
	}

	HashMap(std::initializer_list<value_type> list) : HashMap()
	{
		for(auto elem : list)
			insert(elem);
	}

	HashMap(const HashMap &other) : HashMap(other.buckets)
	{
		for(auto elem : other)
			insert(elem);
	}

	HashMap(HashMap &&other)// : buckets(other.buckets), addedElements(other.addedElements)
	{
		//table = std::move(other.table);
		*this = std::move(other);
	}

	HashMap &operator=(const HashMap &other)
	{
		if(this != &other)
		{
			table.clear();
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
		buckets = std::move(other.buckets);
		addedElements = std::move(other.addedElements);
		table = std::move(other.table);

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
			insert( {key, mapped_type{}} );
			auto index = myHash(key);
			return table[index].back().second;
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
		auto it = find(key);

		if(it == end())
			throw std::out_of_range("key doesn't exist");

		return it->second;
	}

	const_iterator find(const key_type &key) const
	{
		size_t index = myHash(key);

		for(auto it = table[index].begin(); it != table[index].end(); ++it)
		{
			if(it->first == key)
				return ConstIterator(this, index, it);
		}

		return cend();
	}

	iterator find(const key_type &key)
	{
		size_t index = myHash(key);

		for(auto it = table[index].begin(); it != table[index].end(); ++it)
		{
			if(it->first == key)
				return Iterator(ConstIterator(this, index, it));
		}

		return end();
	}

	void remove(const key_type &key)
	{
		auto it = find(key);
		remove(it);
	}

	void remove(const const_iterator &it)
	{
		size_t index = it.hashIndex;

		if(it == end())
			throw std::out_of_range("out of range");

		auto position = it.currentIterator;

		table[index].erase(position);
		--addedElements;
	}

	size_type getSize() const
	{
		return addedElements;
	}

	bool operator==(const HashMap &other) const
	{
		if(addedElements != other.addedElements)
			return false;

		if(addedElements == 0 && other.addedElements == 0)
			return true;

		for(auto elem : other)
		{
			auto it = find(elem.first);
			if( it == end() || *it != elem )
				return false;
		}
		return true;
	}

	bool operator!=(const HashMap &other) const
	{
		return !(*this == other);
	}

	iterator begin()
	{
		return Iterator(cbegin());
	}

	iterator end()
	{
		return Iterator(cend());
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
		return cend();
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
	listIterator currentIterator = {};

	const std::list<value_type> &getTable()
	{
		return map->table[hashIndex];
	}

public:

	explicit ConstIterator() = default;

	ConstIterator(const HashMap *map, size_t hashIndex, listIterator currentIterator={})
			: map(map), hashIndex(hashIndex), currentIterator(currentIterator)
	{}

	ConstIterator(const ConstIterator &other)
			: map(other.map), hashIndex(other.hashIndex), currentIterator(other.currentIterator)
	{}

	ConstIterator &operator++()
	{
		if(map == nullptr)
			throw std::logic_error("collection not given to iterator");

		if(hashIndex == map->buckets)
			throw std::out_of_range("iterator out of range");

		++currentIterator;
		while( currentIterator == getTable().end() ) //skip lists which size is 0
		{
			++hashIndex;

			if(hashIndex == map->buckets) //if incrementing iterator pointing to last element
			{
				*this = map->end();
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

		while( currentIterator == getTable().begin() ) //while end == begin skip this list
		{
			--hashIndex;
			currentIterator = getTable().end();
		}

		--currentIterator; //decrement from end to last element in list
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
		if(hashIndex == map->buckets)
			throw std::out_of_range("out of range");

		return *currentIterator;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	bool operator==(const ConstIterator &other) const
	{
		return currentIterator == other.currentIterator;
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
