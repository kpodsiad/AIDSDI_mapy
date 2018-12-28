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

  std::vector< std::vector<std::pair<int,int> > > vector;
  vector.resize(10);
  for (int i = 0; i < 10 ; ++i)
  {
    vector[i].push_back({i,i});
    vector[i].push_back({i+5,i+5});
    vector[i].push_back({i+10,i+10});
  }

//  auto x = vector[0].at(0);
//  auto y = vector[0][1];
  vector[0].erase(vector[0].end()+1);

  perfomTest();
  return 0;
}
