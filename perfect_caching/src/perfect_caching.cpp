#include <list>
#include <iostream>

#include "perfect_caching.h"

std::identity slow_get_page;

int main()
{
    size_t cache_size, n;

    std::cin >> cache_size >> n;
    if (!std::cin.good())
    {
        std::cerr << "Input error: cache size." << std::endl;
        return 1;
    }

    caches::perfect_cache_t<int, int> pc{cache_size};
    caches::page_calls_list_t<int> calls{n};
    std::cout << pc.calculate_hits(calls, slow_get_page) << std::endl;
    
    return 0;
}