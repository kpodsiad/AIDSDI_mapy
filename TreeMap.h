#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>



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
		value_type value{};
		Node *left{nullptr};
		Node *right{nullptr};
		Node *parent{nullptr};
		int bf{0};

		explicit Node(const value_type &value) : value(value)
		{}

		const key_type &getKey() const
		{
			return value.first;
		}
	};

	size_t size{0};
	Node *root{nullptr};


	void deleteTree(Node *node)
	{
		if(node)
		{
			deleteTree(node->left);
			deleteTree(node->right);
			--size;
			delete node;
		}
	}

	void insert(value_type value)
	{
		Node *p = root, *r;
		Node *newNode = new Node{value};

		if(root == nullptr)
			root = newNode;
		else
		{
			while (true)
			{
				if (value.first < p->value.first)
				{
					if (p->left == nullptr)
					{
						p->left = newNode;
						break;
					}
					p = p->left;
				} else
				{
					if (p->right == nullptr)
					{
						p->right = newNode;
						break;
					}
					p = p->right;
				}

			}
			newNode->parent = p;

			if(p->bf == 1 || p->bf == -1)
				p->bf = 0;
			else
			{
				if(p->left == newNode)
					p->bf = 1;
				else
					p->bf = -1;
			}

			r = p->parent;
			bool t = false;

			while(r != nullptr)
			{
				if(r->bf != 0)
				{
					t = true;
					break;
				}

				if(r->left == p)
					r->bf =  1;
				else
					r->bf = -1;

				p = r;
				r = r->parent;
			}

			if(t)             // jeśli r.bf = +/- 1, to musimy
			{                 // równoważyć drzewo
				if (r->bf == 1)
				{
					if (r->right == p)
						r->bf = 0;  // gałęzie się równoważą
					else if (p->bf == -1)
						rotateLR(r);
					else
						rotateLL(r);
				}
				else
				{              // r.bf = -1
					if (r->left == p)
						r->bf = 0;  // gałęzie się równoważą
					else if (p->bf == 1)
						rotateRL(r);
					else
						rotateRR(r);
				}
			}
		}
		++size;
	}

	void rotateRR(Node *a)
	{
		Node *b = a->right;

		a->right = b->left;
		if(a->right != nullptr)
			a->right->parent = a;

		b->left = a;
		b->parent = a->parent;
		a->parent = b;

		if(b->parent)
		{
			if(b->parent->left == a)
				b->parent->left = b;
			else
				b->parent->right = b;
		}
		else
			root = b;

		if(b->bf == 0) // children of b has the same height
		{
			b->bf = -1;
			a->bf = 1;
		}
		else
			a->bf = b->bf = 0;
	}

	void rotateLL(Node *a)
	{
		Node *b = a->left;

		a->left = b->right;
		if(b->right != nullptr)
			b->right->parent = a;

		b->right = a;
		b->parent = a->parent;
		a->parent = b;

		if(b->parent)
		{
			if(b->parent->left == a)
				b->parent->left = b;
			else
				b->parent->right = b;
		}
		else
			root = b;

		if(b->bf == 0) // children of b has the same height
		{
			b->bf = 1;
			a->bf = -1;
		}
		else
			a->bf = b->bf = 0;
	}

	void rotateRL(Node *a) // a (Right) b (Left) c
	{
		Node *b = a->right;
		Node *c = b->left;

		b->left = c->right;
		if(b->left != nullptr)
			b->left->parent = b;

		c->right = b;
		b->parent = c;

		a->right = c->left;
		if(a->right != nullptr)
			a->right->parent = a;

		c->left = a;
		c->parent = a->parent;
		a->parent = c;

		if(c->parent)
		{
			if(c->parent->left == a)
				c->parent->left = c;
			else
				c->parent->right = c;
		}
		else
			root = c;

		if(c->bf < 0) // c node was left-heavy
		{
			a->bf = 1;
			b->bf = 0;
		}
		else if(c->bf == 0)
		{
			a->bf = 0;
			b->bf = 0;
		}
		else // c node was right-heavy
		{
			a->bf = 0;
			b->bf = -1;
		}

		c->bf = 0;
	}

	void rotateLR(Node *a) // a (Left) b (Right) c
	{
		Node *b = a->left;
		Node *c = b->right;

		b->right = c->left;
		if(b->right != nullptr)
			b->right->parent = b;

		c->left = b;
		b->parent = c;

		a->left = c->right;
		if(a->left != nullptr)
			a->left->parent = a;

		c->right = a;
		c->parent = a->parent;
		a->parent = c;

		if(c->parent)
		{
			if(c->parent->left == a)
				c->parent->left = c;
			else
				c->parent->right = c;
		}
		else
			root = c;

		if(c->bf < 0) // c node was left-heavy
		{
			a->bf = 0;
			b->bf = -1;
		}
		else if(c->bf == 0)
		{
			a->bf = 0;
			b->bf = 0;
		}
		else // c node was right-heavy
		{
			a->bf = 1;
			b->bf = 0;
		}

		c->bf = 0;
	}

	Node* maxNode(Node *node) const
	{
		if(node->right)
			return maxNode(node->right);
		return node;
	}
	Node* minNode(Node *node) const
	{
		if(node && node->left)
			return maxNode(node->left);
		return node;
	}

	Node* previous(Node *p)
	{
		Node *r;

		if(p)
		{
			if(p->left)
			{
				p = p->left;
				while(p->right) p = p->right;
			}
			else
				do
				{
					r = p;
					p = p->parent;
				} while(p && p->right != r);
		}
		return p;
	}


public:

	TreeMap()
	= default;

	TreeMap(std::initializer_list<value_type> list)
	{
		for( auto elem : list)
			insert(elem);
	}

	TreeMap(const TreeMap &other)
	{
		for(auto elem : other)
			insert(elem);
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
				insert(elem);
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
			insert({key, mapped_type{} });
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
		while(node->value.first != key)
		{
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
		auto it = ConstIterator(this, root, node);
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

		delete removeNode(it.current);
	}

	Node* removeNode(Node *x)
	{
		Node *y;
		bool nested = false;

		if (x->right && x->left)
		{
			y = removeNode(previous(x));
		}
		else
		{
			if (x->left)
			{
				y = x->left;
				x->left = nullptr;
			}
			else
			{
				y = x->right;
				x->right = nullptr;
			}
			x->bf = 0;
			nested = true;
		}

		if (y)
		{
			y->parent = x->parent;

			y->left = x->left;
			if (y->left)
				y->left->parent = y;

			y->right = x->right;
			if (y->right)
				y->right->parent = y;
		}

		if (x->parent)
		{
			if (x->parent->left == x)
				x->parent->left = y;
			else
				x->parent->right = y;
		} else
			root = y;


		if (nested)
		{
			Node *z = y, *t;
			y = x->parent;
			while (y)
			{
				if (!y->bf)
				{
					if (y->left == z)
						y->bf = -1;
					else
						y->bf = 1;
					break;
				}
				else
				{
					if ((y->bf == 1 && y->left == z) || (y->bf == -1 && y->right == z))
					{
						y->bf = 0;
						z = y;
						y = y->parent;
					}
					else
					{
						if (y->left == z)
							t = y->right;
						else
							t = y->left;
						if (!t->bf)
						{         // Przypadek 3A
							if (y->bf == 1)
								rotateLL(y);
							else
								rotateRR(y);
							break;
						}
						else if (y->bf == t->bf)
						{         // Przypadek 3B
							if (y->bf == 1)
								rotateLL(y);
							else
								rotateRR(y);

							z = t;
							y = t->parent;
						}
						else
						{         // Przypadek 3C
							if (y->bf == 1)
								rotateLR(y);
							else
								rotateRL(y);

							z = y->parent;
							y = z->parent;
						}
					}

				}
			}


		}
		--size;
		return x;
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
		if(size != other.size)
			return false;
		auto it = begin();
		for(auto elemValue : other)
		{
			if(it.current->value != elemValue)
				return false;
			++it;
		}
		return  true;
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
		return ConstIterator{this, root, minNode(root)};
	}

	const_iterator cend() const
	{
		return ConstIterator{this, root, nullptr};
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

	explicit ConstIterator()
	= default;

	friend class TreeMap;

	ConstIterator(const TreeMap *tree, Node *root, Node *current) : tree(tree), root(root), current(current)
	{}

private:
	const TreeMap *tree;
	Node *root;
	Node *current;
public:

	ConstIterator(const ConstIterator &other): tree(other.tree), root(other.root), current(other.current)
	{
	}

	ConstIterator &operator++()
	{
		if(current == nullptr)
			throw std::out_of_range("out of range");
		if(root == nullptr)
			throw std::out_of_range("Collection is empty");

		if(current->right)
		{
			current = current->right;
			while(current->left)
				current = current->left;
		}
		else
		{
			Node *parent = current->parent;
			while(parent && parent->right == current)
			{
				current = parent;
				parent = current->parent;
			}
				current = parent;
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
		Node *r = root;
		while(r->left)
			r = r->left;
		if(current == r)
			throw std::out_of_range("out of range");

		if(current == nullptr)
		{
			current = root;
			while(current->right)
				current = current->right;
		}
		else if(current->left)
			current = current->left;
		else
		{
			Node *parent = current->parent;
			while(parent && parent->left == current)
			{
				current = parent;
				parent = current->parent;
			}
			current = parent;
		}
		return *this;
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
		return root == other.root && current == other.current;
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
