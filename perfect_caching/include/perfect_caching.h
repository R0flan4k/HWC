#ifndef PERFECT_CACHING_H
    #define PERFERCT_CACHING_H

    #include <map>
    #include <unordered_map>
    #include <cassert>

    namespace caches {

    template <typename KeyT> class page_calls_list_t {
        size_t sz_;
    
    public:
        std::unordered_map<KeyT, int> freqs_map_;
        std::list<KeyT> pages_;

        size_t size() const {return sz_;}

        template <class RandomIt> explicit page_calls_list_t(size_t sz,
                                                             RandomIt first, RandomIt last)
        : sz_(sz)
        {
            for (; first != last; first++)
            {
                KeyT cur_key = *first;
                pages_.push_back(cur_key);

                auto hit = freqs_map_.find(cur_key);
                if (hit == freqs_map_.end())
                {
                    freqs_map_.emplace(cur_key, 1);
                }
                else
                {
                    (hit->second)++;
                }
            }
        }
    };

    template <typename T, typename KeyT, typename PageCallT> class perfect_cache_t {
        size_t sz_;
        std::multimap<int, KeyT> abs_freqs_;

        using MapIt = typename std::multimap<int, KeyT>::iterator;
        struct map_entry_t {
            MapIt map_it;
            int freq = 0;
            T entry;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

        PageCallT slow_get_page_;

    public:
        bool full() const {return hash_.size() == sz_;}

    private:
        bool caches_update(const KeyT &key, const int freq)
        {
            auto hit = hash_.find(key);
            if (hit == hash_.end())
            {
                if (full())
                {
                    hash_.erase(abs_freqs_.begin()->second);
                    abs_freqs_.erase(abs_freqs_.begin());
                }

                hash_.try_emplace(key, abs_freqs_.emplace(freq, key), freq, slow_get_page_(key));
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
        explicit perfect_cache_t(size_t sz, PageCallT slow_get_page) 
        : sz_(sz), slow_get_page_(slow_get_page) {}

        int calculate_hits(const page_calls_list_t<KeyT> &calls)
        {
            auto lst_it = calls.pages_.begin();

            int hits = 0;
            for (size_t i = 0; i < calls.size(); i++)
            {
                hits += caches_update(*lst_it, calls.freqs_map_.find(*lst_it)->second);
                lst_it++;
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H