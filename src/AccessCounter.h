#pragma once
#include <unordered_map>
#include <cinttypes>
class AccessCounter
{

    std::unordered_map<std::uint32_t, std::uint32_t> counterMap;

  public:
    AccessCounter();

    void increment(std::uint32_t index);
    void decrement(std::uint32_t index);
    std::uint32_t getCount(std::uint32_t index);
};