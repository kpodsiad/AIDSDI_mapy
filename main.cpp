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

  (void)argc;
  (void)argv;

  Map<int,std::string> map = { { 42, "Chuck" }, { 27, "Bob" } };
  //Map<int,int> other = {};

  auto it = map.find(42);
  it->second = "Alice";
  auto begin = map.begin();
  auto end = map.end();



  auto x = begin == end;

  if(x)
    std::cout<<"hehe";
  perfomTest();
  return 0;
}
