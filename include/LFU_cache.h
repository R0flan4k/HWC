#ifndef LFU_CACHE_H
    #define LFU_CACHE_H

    #include <list>
    #include <unordered_map>
    #include <map>

    namespace caches {

    template <typename T, typename KeyT = int> class cache_t {
        size_t sz_;
        std::multimap<unsigned int, KeyT> freqs_;

        using MapIt = typename std::multimap<unsigned int, KeyT>::iterator;
        struct map_entry_t {
            unsigned int freq = 0;
            T entry;
            MapIt map_it;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

    public:
        cache_t(size_t sz) {sz_ = sz;}

        bool full() {return hash_.size() == sz_;}

        template <typename F> bool lookup_update(KeyT key, F slow_get_page) 
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(freqs_.begin()->second);
                    freqs_.erase(freqs_.begin());
                }

                map_entry_t new_val = {1, slow_get_page(key), freqs_.emplace(1, key)};
                hash_.emplace(key, new_val); // First page call.
                return false;
            }

            auto elt_it = &(hit->second);
            freqs_.erase(elt_it->map_it);
            elt_it->freq += 1;
            elt_it->map_it = freqs_.emplace(elt_it->freq, key);
            return true;
        }
    };
    } // namespace caches
    
#endif