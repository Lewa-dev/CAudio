#include "AccessCounter.h"

AccessCounter::AccessCounter()
{
}

void AccessCounter::increment(unsigned int index)
{
    auto it = counterMap.find(index);
    if (it != counterMap.end())
    {
        it->second++;
    }
    else
    {
        counterMap.insert(std::make_pair(index, 1));
    }
}

void AccessCounter::decrement(unsigned int index)
{
    auto it = counterMap.find(index);
    if (it != counterMap.end())
    {
        it->second--;
        if (it->second == 0)
        {
            counterMap.erase(it);
        }
    }
}

unsigned int AccessCounter::getCount(unsigned int index)
{
    auto it = counterMap.find(index);
    if (it != counterMap.end())
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}
