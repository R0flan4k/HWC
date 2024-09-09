#include <iostream>
#include <cassert>

#include "LFU_cache.h"

int slow_get_page(int key) {return key;}

int main()
{
    size_t cache_size = 0;
    int hits = 0, n = 0;

    std::cin >> cache_size >> n;
    if (!std::cin.good())
    {
        std::cout << "Input error." << std::endl;;
        return 1;
    }

    caches::cache_t<int> lfu{cache_size};

    for (int i = 0; i < n; i++)
    {
        int q = 0;
        std::cin >> q;
        assert(std::cin.good());
        if (lfu.lookup_update(q, slow_get_page))
            hits += 1;
    }

    std::cout << hits << std::endl;
    return 0;
}