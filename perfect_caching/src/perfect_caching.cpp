#include "perfect_caching.h"

#include <iostream>
#include <iterator>
#include <functional>

int main()
{
    size_t cache_size, n;

    std::cin >> cache_size >> n;
    if (!std::cin.good())
    {
        std::cerr << "Input error: cache size." << std::endl;
        return 1;
    }

    caches::perfect_cache_t<int, int, std::identity> pc{cache_size, std::identity{}};
    caches::page_calls_list_t<int> calls{n, std::istream_iterator<int>(std::cin), std::istream_iterator<int>()};
    std::cout << pc.calculate_hits(calls) << std::endl;
    
    return 0;
}