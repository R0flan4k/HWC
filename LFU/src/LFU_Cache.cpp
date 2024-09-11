#include <iostream>
#include <cassert>

#include "LFU_cache.h"

std::_Identity<int> slow_get_page;

int main()
{
    size_t cache_size;
    int n;

    std::cin >> cache_size >> n;
    if (!std::cin.good())
    {
        std::cout << "Input error: cache size." << std::endl;
        return 1;
    }

    caches::lfu_t<int, int> lfu{cache_size};
    int hits = 0;

    for (int i = 0; i < n; i++)
    {
        int q;
        std::cin >> q;
        if (!std::cin.good())
        {
            std::cout << "Input error: data N" << i << "." << std::endl;;
            return 1;
        }
        hits += lfu.lookup_update(q, slow_get_page);
    }

    std::cout << hits << std::endl;
    return 0;
}