#include <iostream>
#include <cassert>
#include <functional>

#include "LFU_cache.h"

int main()
{
    size_t cache_size;
    size_t n;

    std::cin >> cache_size >> n;
    if (!std::cin.good())
    {
        std::cerr << "Input error: cache size." << std::endl;
        return 1;
    }

    caches::lfu_t<int, int, std::identity> lfu{cache_size, std::identity{}};
    int hits = 0;

    for (size_t i = 0; i < n; i++)
    {
        int q;
        std::cin >> q;
        if (!std::cin.good())
        {
            std::cerr << "Input error: data N " << i << "." << std::endl;
            return 1;
        }
        hits += lfu.lookup_update(q);
    }

    std::cout << hits << std::endl;
    return 0;
}