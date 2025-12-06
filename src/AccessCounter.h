#pragma once
#include <map>
class AccessCounter
{

    std::map<unsigned int, unsigned int> counterMap;

  public:
    AccessCounter();

    void increment(unsigned int index);
    void decrement(unsigned int index);
    unsigned int getCount(unsigned int index);
};