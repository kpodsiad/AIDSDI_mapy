#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>
#include <stack>



namespace aisdi
{
	template<typename KeyType, typename ValueType>
class TreeMap
{
public:
	using key_type = KeyType;
	using mapped_type = ValueType;
	using value_type = std::pair<const key_type, mapped_type>;
	using size_type = std::size_t;
	using reference = value_type &;
	using const_reference = const value_type &;

	class ConstIterator;

	class Iterator;

	using iterator = Iterator;
	using const_iterator = ConstIterator;

public:
	struct Node
	{
	private:
		value_type value{};
		Node *left{nullptr};
		Node *right{nullptr};
		int height{1};

		friend class TreeMap;

	public:

		explicit Node(const value_type &value) : value(value)
		{}

		Node &operator=(Node &other)
		{
			value = other.value;
			return *this;
		}
		const key_type &getKey() const
		{
			return value.first;
		}

		int getHeight()
		{
			if (this == nullptr)
				return 0;

			return this->height;
		}

		int getBalanceFactor()
		{
			return this->left->getHeight() - this->right->getHeight();
		}

		Node* max()
		{
			Node *temp = this;
			while(temp->right)
				temp = temp->right;
			return temp;
		}

		Node* min()
		{
			Node *temp = this;
			while(temp->left)
				temp = temp->left;
			return temp;
		}
	};

		size_t size{0};
		Node *root{nullptr};

		int max(int a, int b)
		{
			return a > b ? a : b;
		}

		void updateNodeHeight(Node *node)
		{
			node->height = 1 + max(getHeight(node->left), getHeight(node->right));
		}

		int getHeight(Node *node)
		{
			if (node == nullptr)
				return 0;

			return node->height;
		}

		int getBalanceFactor(Node *node)
		{
			return getHeight(node->left) - getHeight(node->right);
		}

		void deleteTree(Node *node)
		{
			if (node)
			{
				deleteTree(node->left);
				deleteTree(node->right);
				--size;
				delete node;
			}
		}

		Node *findSmallest(Node *node)
		{
			Node *currentNode = node;
			while (currentNode->left != nullptr)
				currentNode = currentNode->left;

			return currentNode;
		}

		Node *rotateRR(Node *a)
		{
			Node *b = a->right;

			a->right = b->left;
			b->left = a;

			updateNodeHeight(a);
			updateNodeHeight(b);

			return b;
		}

		Node *rotateLL(Node *a)
		{
			Node *b = a->left;

			a->left = b->right;
			b->right = a;

			updateNodeHeight(a);
			updateNodeHeight(b);

			return b;
		}

		Node *rotateRL(Node *a)
		{
			a->right = rotateLL(a->right);
			return rotateRR(a);
		}

		Node *rotateLR(Node *a)
		{
			a->left = rotateRR(a->left);
			return rotateLL(a);
		}

		Node *insert(Node *node, value_type value)
		{
			if (node == nullptr) //this is place where we insert new Node
			{
				++size;
				return (new Node{value});
			}

			key_type key = value.first;
			if (key < (node->getKey()) ) //search proper place
				node->left = insert(node->left, value);
			else
				node->right = insert(node->right, value);

			updateNodeHeight(node); //update height of node

			int bf = getBalanceFactor(node); //get node balance factor

			//if bf<-1 || bf>1 we must do a rotation

			if (bf > 1 && key < node->left->getKey())
				return rotateLL(node);

			if (bf > 1 && key > node->left->getKey())
			{
				node->left = rotateRR(node->left);
				return rotateLL(node);
			}

			if (bf < -1 && key > node->right->getKey())
				return rotateRR(node);

			if (bf < -1 && key < node->right->getKey())
			{
				node->right = rotateLL(node->right);
				return rotateRR(node);
			}

			return node;
		}

		Node *deleteNode(Node *node, key_type key)
		{
			if (node == nullptr)
				return node;

			if (key < node->getKey())
				node->left = deleteNode(node->left, key);
			else if (key > node->getKey())
				node->right = deleteNode(node->right, key);
			else
			{
				if (node->left == nullptr || node->right == nullptr) // 0 or 1 children
				{
					Node *temp = node;
					if (node->left != nullptr)
						node = node->left;
					else
						node = node->right;

					delete temp;
				}
				else // 2 children
				{
					Node *smallest = findSmallest(node->right);

					auto *ptr = const_cast<key_type *>(&node->value.first);
					*ptr = smallest->value.first;
					node->value.second = smallest->value.second;

					node->right = deleteNode(node->right, smallest->getKey());
				}
				--size;
			}
			if (node == nullptr)
				return nullptr;

			updateNodeHeight(node);

			int bf = getBalanceFactor(node);
			if(bf < -1)
			{
				if(getBalanceFactor(node->right) < 0)
					return  rotateRR(node);
				else
					return  rotateRL(node);
			}
			else if (bf > 1)
			{
				if(getBalanceFactor(node->left) > 0)
					return rotateLL(node);
				else
					return rotateLR(node);
			}

			return node;
		}

public:

	TreeMap()
	= default;

	TreeMap(std::initializer_list<value_type> list)
	{
		for( auto elem : list)
			root = insert(root, elem);
	}

	TreeMap(const TreeMap &other)
	{
		for(auto elem : other)
			root = insert(root, elem);
	}

	TreeMap(TreeMap &&other) : size(other.size), root(other.root)
	{
		other.root = nullptr;
		other.size = 0;
	}

	~TreeMap()
	{
		deleteTree(root);
		root = nullptr;
	}

	TreeMap &operator=(const TreeMap &other)
	{
		if(this != &other)
		{
			deleteTree(root);
			root = nullptr;
			for(auto elem : other)
				root = insert(root, elem);
		}
		return *this;
	}

	TreeMap &operator=(TreeMap &&other)
	{
		if(this != &other)
		{
			deleteTree(root);
			root = nullptr;
			size = other.size;
			root = other.root;
			other.size = 0;
			other.root = nullptr;
		}
		return *this;
	}

	bool isEmpty() const
	{
		return size == 0;
	}

	mapped_type &operator[](const key_type &key)
	{
		auto it = find(key);
		if(it == end() )
		{
			root = insert(root, {key, mapped_type{} });
			return find(key)->second;
		}
		return it->second;
	}

	const mapped_type &valueOf(const key_type &key) const
	{
		if(isEmpty())
			throw std::out_of_range("Collection is empty");
		auto it = find(key);
		if(it == end())
			throw std::out_of_range("Key doesn't exist");

		return it->second;
	}

	mapped_type &valueOf(const key_type &key)
	{
		if(isEmpty())
			throw std::out_of_range("Collection is empty");
		auto it = find(key);
		if(it == end())
			throw std::out_of_range("Key doesn't exist");
		return it->second;
	}

	const_iterator find(const key_type &key) const
	{
		if(root == nullptr)
			return cend();

		Node *node = root;
		std::stack<Node*> up;
		while(node->value.first != key)
		{
			up.push(node);
			if(key < node->value.first)
			{
				if(node->left)
					node = node->left;
				else
					return cend();
			}

			if(key > node->value.first)
			{
				if(node->right)
					node = node->right;
				else
					return cend();
			}
		}
		auto it = ConstIterator(root, node, up);
		return it;
	}

	iterator find(const key_type &key)
	{
		return Iterator( (const_cast<const TreeMap *>(this))->find(key));
	}

	void remove(const key_type &key)
	{
		if(root == nullptr)
			throw std::out_of_range("Collection is empty");

		auto it = find(key);
		if(it.current == nullptr)
			throw std::out_of_range("there isn't element with that key");

		root = deleteNode(root, key);
	}

	void remove(const const_iterator &it)
	{
		remove(it->first);
	}

	size_type getSize() const
	{
		return size;
	}

	bool operator==(const TreeMap &other) const
	{
		if(size != other.size )
			return false;

		for(auto elem : *this)
		{
			auto it = other.find(elem.first);
			auto endIt = other.end();
			if(it == endIt || elem != *it)
				return false;
		}
		for(auto elem : other)
		{
			auto it = find(elem.first);
			auto endIt = end();
			if(it == endIt || elem != *it)
				return false;
		}
		return true;
	}

	bool operator!=(const TreeMap &other) const
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
		if(size == 0)
			return cend();

		std::stack<Node *> up;
		Node *node = root;
		while(node->left != nullptr)
		{
			up.push(node);
			node = node->left;
		}
		return ConstIterator{root, node, up};
	}

	const_iterator cend() const
	{
		return ConstIterator{root, nullptr};
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename TreeMap::const_reference;
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename TreeMap::value_type;
	using pointer = const typename TreeMap::value_type *;
	using Node = typename TreeMap::Node;

	explicit ConstIterator()
	= default;

	friend class TreeMap;


private:
	Node *root;
	Node *current;
	std::stack<Node*> up;

public:
	ConstIterator(Node *root, Node *current, std::stack<Node*> &up)
	:root(root), current(current), up(up)
	{}

	ConstIterator(Node *root, Node *current) :root(root), current(current)
	{
		Node *temp = root;
		if(current != nullptr)
		{
			while(temp != current)
			{
				up.push(temp);
				if(current->getKey() < temp->getKey())
					temp = temp->left;
				else
					temp = temp->right;
			}
		}
		else
		{
			while(temp != current)
			{
				up.push(temp);
				temp = temp->right;
			}
		}
	}

	ConstIterator(const ConstIterator &other):root(other.root), current(other.current), up(other.up)
	{
	}

	ConstIterator &operator++()
	{
		if(root == nullptr)
			throw std::out_of_range("Collection is empty");
		if(current == nullptr)
			throw std::out_of_range("out of range incrementing");

		if(current == root->max() )
		{
			up.push(current);
			current = nullptr;
			return *this;
		}

		if(current->right != nullptr) //has right son
		{
			up.push(current);
			current = current->right->min();
			return *this;
		}

		if(current->right == nullptr) //doesnt have right son, must travel up until met greater
		{
			Node *temp = current;
			current = up.top();
			up.pop();
			while( temp == current->right && !up.empty() ) //while temp is bigger and stack isn't empty
			{
				temp = current;
				current = up.top();
				up.pop();
			}
			if( current == root && temp->getKey() > current->getKey() ) //we travel from right side and not found bigger
				current = nullptr;

			return *this;
		}
		return *this;
	}

	const ConstIterator operator++(int)
	{
		auto it = *this;
		operator++();
		return it;
	}

	ConstIterator &operator--()
	{
		if(root == nullptr)
			throw std::out_of_range("Collection is empty");
		if(current == root->min() )
			throw std::out_of_range("out of range decrementing");
		if(current == nullptr)
		{
			current = up.top();
			up.pop();
			return *this;
		}

		if(current->left != nullptr)
		{
			up.push(current);
			current = current->left->max();
			return *this;
		}
		else
		{
			Node *temp = current;
			while(temp != current->left)
			{
				temp = current;
				current = up.top();
				up.pop();
			}
			return *this;
		}

	}

	const ConstIterator operator--(int)
	{
		auto it = *this;
		operator--();
		return it;
	}

	reference operator*() const
	{
		if(root == nullptr)
			throw std::out_of_range("collection is empty");
		if(current == nullptr)
			throw std::out_of_range("out of range");

		return current->value;
	}

	pointer operator->() const
	{
		return &this->operator*();
	}

	bool operator==(const ConstIterator &other) const
	{
		return up == other.up && current == other.current;
	}

	bool operator!=(const ConstIterator &other) const
	{
		return !(*this == other);
	}


};

template<typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
	using reference = typename TreeMap::reference;
	using pointer = typename TreeMap::value_type *;

	explicit Iterator()
	= default;

	explicit Iterator(const ConstIterator &other)
			: ConstIterator(other)
	{}

	Iterator &operator++()
	{
		ConstIterator::operator++();
		return *this;
	}

	const Iterator operator++(int)
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

	const Iterator operator--(int)
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

#endif /* AISDI_MAPS_MAP_H */
