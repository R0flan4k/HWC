#ifndef PERFECT_CACHING_H
    #define PERFERCT_CACHING_H

    #include <map>
    #include <unordered_map>
    #include <cassert>

    namespace caches {

    template <typename KeyT> class page_calls_list_t {
        size_t sz_;
    
    public:
        std::unordered_map<KeyT, unsigned int> freqs_map_;
        std::list<KeyT> pages_;

        size_t size() const {return sz_;}

        explicit page_calls_list_t(size_t sz)
        : sz_(sz)
        {
            for (int i = 0; i < sz; i++)
            {
                KeyT key;
                std::cin >> key;
                pages_.push_back(key);

                auto hit = freqs_map_.find(key);
                if (hit == freqs_map_.end())
                {
                    freqs_map_.emplace(key, 1);
                    continue;
                }
                (hit->second)++;
            }
        }
    };

    template <typename T, typename KeyT> class perfect_cache_t {
        size_t sz_;
        std::multimap<unsigned int, KeyT> abs_freqs_;

        using MapIt = typename std::multimap<unsigned int, KeyT>::iterator;
        struct map_entry_t {
            unsigned int freq = 0;
            T entry;
            MapIt map_it;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

    public:
        bool full() const {return hash_.size() == sz_;}

    private:
        template <typename F> bool caches_update(const KeyT &key, const unsigned int freq,
                                                 F slow_get_page)
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(abs_freqs_.begin()->second);
                    abs_freqs_.erase(abs_freqs_.begin());
                }

                hash_.try_emplace(key, freq, slow_get_page(key), abs_freqs_.emplace(freq, key));
                return false;
            }

            // Expect frequency of page calculated beforehand.
            map_entry_t &elt = hit->second;
            abs_freqs_.erase(elt.map_it);
            elt.freq -= 1;
            assert(elt.freq >= 0);
            elt.map_it = abs_freqs_.emplace(elt.freq, key);
            return true;
        }

    public:
        explicit perfect_cache_t(size_t sz) : sz_(sz) {};

        template <typename F> int calculate_hits(const page_calls_list_t<KeyT> &calls,
                                                 F slow_get_page)
        {
            auto lst_it = calls.pages_.begin();

            int hits = 0;
            for (size_t i = 0; i < calls.size(); i++)
            {
                hits += caches_update(*lst_it, calls.freqs_map_.find(*lst_it)->second,
                                      slow_get_page);
                lst_it = std::next(lst_it);
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H