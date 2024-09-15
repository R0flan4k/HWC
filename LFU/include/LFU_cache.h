#ifndef LFU_CACHE_H
    #define LFU_CACHE_H

    #include <unordered_map>
    #include <map>

    namespace caches {

    template <typename T, typename KeyT, typename PageCallT> class lfu_t {
        size_t sz_;
        std::multimap<unsigned, KeyT> freqs_;

        using MapIt = typename std::multimap<unsigned, KeyT>::iterator;
        struct map_entry_t {
	        MapIt map_it;
            unsigned freq = 0;
            T entry;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

        PageCallT slow_get_page_;

    public:
        explicit lfu_t(size_t sz, PageCallT slow_get_page) 
        : sz_(sz), slow_get_page_(slow_get_page) {}

        bool full() const {return hash_.size() == sz_;}

        bool lookup_update(const KeyT &key) 
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(freqs_.begin()->second);
                    freqs_.erase(freqs_.begin());
                }

                hash_.try_emplace(key, freqs_.emplace(1, key), 1, slow_get_page_(key)); 
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
