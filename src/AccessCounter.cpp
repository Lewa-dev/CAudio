#include "AccessCounter.h"

AccessCounter::AccessCounter()
{
}

void AccessCounter::increment(std::uint32_t index)
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

void AccessCounter::decrement(std::uint32_t index)
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

std::uint32_t AccessCounter::getCount(std::uint32_t index)
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
