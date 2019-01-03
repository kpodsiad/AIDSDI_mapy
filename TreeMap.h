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

	void print()
	{
		print(root);
	}

private:
	class Node;

	size_t size{0};
	Node *root{nullptr};

	class Node
	{
		friend class TreeMap;

	private:
		value_type *value{nullptr};
		Node *left{nullptr};
		Node *right{nullptr};
		int height{1};

		bool hasBothChildren() const
		{ return left != nullptr && right != nullptr; }

		bool hasRightChild() const
		{ return right != nullptr; }

		bool hasLeftChild() const
		{ return left != nullptr; }

		int max(int a, int b)
		{
			return a > b ? a : b;
		}

	public:

		explicit Node(const value_type &value)
		{
			this->value = new value_type{value};
		}

		~Node()
		{
			delete value;
		}

		const key_type &getKey() const
		{
			return value->first;
		}

		bool operator==(const Node &other)
		{
			return *value == *other.value;
		}

		bool operator!=(const Node &other)
		{
			return !(this->operator==(other));
		}

		void updateHeight()
		{
			if(hasBothChildren() )
				this->height = 1 + max( this->left->height, this->right->height );

			else if(hasLeftChild() )
				this->height = 1 + this->left->height;

			else if(hasRightChild() )
				this->height = 1 + this->right->height;

			else //doesn't have children
				this->height = 1;
		}

		int getBalanceFactor()
		{
			if(hasBothChildren() )
				return (this->left->height - this->right->height);

			else if(hasLeftChild() )
				return this->left->height;

			else if(hasRightChild() )
				return -1*this->right->height;

			else //doesn't have children
				return 0;
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
			while (temp->left)
				temp = temp->left;
			return temp;
		}
	};//node class

		void print(Node *node)
		{
			if(node == nullptr)
				return;

			if(node->left == nullptr && node->right == nullptr)
				std::cout<<node->value->first<<"->("<<"null"<<","<<"null"<<")"<<"\n";
			else if(node->left != nullptr && node->right != nullptr)
				std::cout<<node->value->first<<"->("<<node->left->value->first<<","<<node->right->value->first<<")"<<"\n";
			else if(node->left != nullptr)
				std::cout<<node->value->first<<"->("<<node->left->value->first<<","<<"null"<<")"<<"\n";
			else
				std::cout<<node->value->first<<"->("<<"null"<<","<<node->right->value->first<<")"<<"\n";
			print(node->left);
			print(node->right);
		}

		void deleteTree()
		{
			deleteTree(root);
			root = nullptr;
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

			a->updateHeight();
			b->updateHeight();

			return b;
		}

		Node *rotateLL(Node *a)
		{
			Node *b = a->left;

			a->left = b->right;
			b->right = a;

			a->updateHeight();
			b->updateHeight();

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

			if (key < node->getKey() ) //search proper place
				node->left = insert(node->left, value);
			else
				node->right = insert(node->right, value);

			node->updateHeight(); //update height of node

			return performRotation(node);
		}

		Node* performRotation(Node *node)
		{
			int bf = node->getBalanceFactor();

			if(bf > 1) //left child is heavier
			{
				if(node->left->getBalanceFactor() > 0 ) // LL case
					return rotateLL(node);
				else
					return rotateLR(node);  // LR case
			}

			if(bf < -1) //right child is heavier
			{
				if(node->right->getBalanceFactor() < 0) // RR case //key > node->right->getKey()
					return rotateRR(node);
				else
					return rotateRL(node); // RL case
			}

			return node;
		}

		Node *deleteNode(Node *node, key_type key)
		{
			if (node == nullptr)
				return node;

			if (key < node->getKey())
				node->left = deleteNode(node->left, key);
			else if(key > node->getKey())
				node->right = deleteNode(node->right, key);
			else
			{
				if( node->hasBothChildren() ) // 2 children
				{
					Node *smallest = findSmallest(node->right);

					std::swap( node->value, smallest->value);
					node->right = deleteNode(node->right, smallest->getKey());
				}
				else // 1 or 0 children
				{
					Node *temp = node;
					if( node->hasLeftChild() )
						node = node->left;
					else
						node = node->right;

					delete temp;
				}
				--size;
			}
			if (node == nullptr)
				return nullptr;

			node->updateHeight();
			return performRotation(node);
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
			deleteTree();
			for(auto elem : other)
				root = insert(root, elem);
		}
		return *this;
	}

	TreeMap &operator=(TreeMap &&other)
	{
		if(this != &other)
		{
			deleteTree();
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
		while(node->value->first != key)
		{
			up.push(node);
			if(key < node->value->first)
			{
				if(node->left)
					node = node->left;
				else
					return ConstIterator(root, nullptr, up);
			}

			if(key > node->value->first)
			{
				if(node->right)
					node = node->right;
				else
					return ConstIterator(root, nullptr, up);
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

		auto endIt = end();
		for(auto elem : other)
		{
			auto it = find(elem.first);

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

	void goToMinNode()
	{
		while(current->left)
		{
			up.push(current);
			current = current->left;
		}
	}

	void goToMaxNode()
	{
		while(current->right != nullptr)
		{
			up.push(current);
			current = current->right;
		}
	}

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

		else if(current->hasRightChild() ) //has right son
		{
			up.push(current);
			current = current->right;
			goToMinNode();
			return *this;
		}

		else //doesnt have right son, must travel up until meet greater
		{
			Node *temp = current;
			current = up.top();
			up.pop();
			while( temp != current->left ) //while temp is bigger go up and change current into parent
			{
				temp = current;
				current = up.top();
				up.pop();
			}
			return *this;
		}
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

		if(current->hasLeftChild() )
		{
			up.push(current);
			current = current->left;
			goToMaxNode();
			return *this;
		}
		else
		{
			Node *temp;
			do //while temp is bigger go up and change current into parent
			{
				temp = current;
				current = up.top();
				up.pop();
			}while(temp != current->right );
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

		return *(current->value);
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