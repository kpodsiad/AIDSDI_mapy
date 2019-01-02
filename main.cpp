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
  Map<int, std::string> map;
  map.table[0];
  std::cout<<"";

}

} // namespace

int main(int argc, char** argv)
{
//  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
//  for (std::size_t i = 0; i < repeatCount; ++i)
//    perfomTest();

//avl<int,int> tree = {{1,5},{2,10}}, tree2 = {{1,10},{2,5}};
//tree.root = tree.insert(tree.root, {5,5});
//tree.root = tree.insert(tree.root, {3,3});
//tree.root = tree.insert(tree.root, {4,4});
//tree.root = tree.insert(tree.root, {1,1});
//auto it = tree.begin();
//auto it2 = tree.end();
//--it2;


//tree.root = tree.deleteNode(tree.root, 1);

//tree[int{}] = int{};

Map<int, int> map;
  map[int{}] = int{};


  (void)argc;
  (void)argv;

  perfomTest();
  return 0;
}
