#ifndef LFU_CACHE_H
    #define LFU_CACHE_H

    #include <unordered_map>
    #include <map>

    namespace caches {

    template <typename T, typename KeyT> class lfu_t {
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
        explicit lfu_t(size_t sz) : sz_(sz) {}

        bool full() const {return hash_.size() == sz_;}

        template <typename F> bool lookup_update(const KeyT &key, F slow_get_page) 
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(freqs_.begin()->second);
                    freqs_.erase(freqs_.begin());
                }

                hash_.try_emplace(key, 1, slow_get_page(key), freqs_.emplace(1, key)); 
                return false; // First page call.
            }

            map_entry_t &elt = hit->second;
            freqs_.erase(elt.map_it);
            elt.freq += 1;
            elt.map_it = freqs_.emplace(elt.freq, key);
            return true;
        }
    };
    } // namespace caches
    
#endif