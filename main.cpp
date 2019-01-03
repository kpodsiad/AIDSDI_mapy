#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>

#include "TreeMap.h"
#include "HashMap.h"

namespace
{

	template <typename K, typename V>
	using Map = aisdi::HashMap<K, V>;

	template <typename K, typename V>
	using avl = aisdi::TreeMap<K, V>;

	void perfomTest()
	{
		std::cout<<"";

	}

} // namespace

int main(int argc, char** argv)
{
//  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
//  for (std::size_t i = 0; i < repeatCount; ++i)
//    perfomTest();

	avl<int,int> tree = {{1,5},{2,10},{0,0},{4,4},{7,7},{5,5},{3,3}};

	tree.print();




//tree.root = tree.deleteNode(tree.root, 1);

//tree[int{}] = int{};

	Map<int, int> map;
	map[int{}] = int{};


	(void)argc;
	(void)argv;

	perfomTest();
	return 0;
}
