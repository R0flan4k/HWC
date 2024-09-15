#ifndef PERFECT_CACHING_H
    #define PERFERCT_CACHING_H

    #include <map>
    #include <unordered_map>
    #include <list>

    namespace caches {

    template <typename KeyT> class page_calls_list_t {
        size_t sz_;
    
    public:
        std::unordered_map<KeyT, unsigned> freqs_map;
        std::list<KeyT> pages;

        size_t size() const {return sz_;}

        template <class RandomIt> explicit page_calls_list_t(size_t sz,
                                                             RandomIt first, RandomIt last)
        : sz_(sz)
        {
            for (; first != last; first++)
            {
                KeyT cur_key = *first;
                pages.push_back(cur_key);

                auto hit = freqs_map.find(cur_key);
                if (hit == freqs_map.end())
                {
                    freqs_map.emplace(cur_key, 1);
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
        std::multimap<unsigned, KeyT> abs_freqs_;

        using MapIt = typename std::multimap<unsigned, KeyT>::iterator;
        struct map_entry_t {
            MapIt map_it;
            unsigned freq = 0;
            T entry;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

        PageCallT slow_get_page_;

    public:
        bool full() const {return hash_.size() == sz_;}

    private:
        template <class FreqsMapT> bool caches_update(const KeyT &key, FreqsMapT &freqs)
        {
            auto hit = hash_.find(key);
            unsigned new_freq = --(freqs.find(key)->second); // Expect that we can see a future
                                                             // so freqs.find() is always founded.
            if (hit == hash_.end())
            {
                if (full() && new_freq > abs_freqs_.begin()->first)
                {
                    hash_.erase(abs_freqs_.begin()->second);
                    abs_freqs_.erase(abs_freqs_.begin());
                    hash_.try_emplace(key, abs_freqs_.emplace(new_freq, key), new_freq, slow_get_page_(key));
                }
                else if (!full())
                {
                    hash_.try_emplace(key, abs_freqs_.emplace(new_freq, key), new_freq, slow_get_page_(key));
                }
                return false;
            }

            map_entry_t &elt = hit->second;
            abs_freqs_.erase(elt.map_it);
            elt.map_it = abs_freqs_.emplace(new_freq, key);
            elt.freq  = new_freq;
            return true;
        }

    public:
        explicit perfect_cache_t(size_t sz, PageCallT slow_get_page) 
        : sz_(sz), slow_get_page_(slow_get_page) {}

        int calculate_hits(page_calls_list_t<KeyT> &calls)
        {
            auto lst_it = calls.pages.begin();

            int hits = 0;
            for (size_t i = 0; i < calls.size(); i++)
            {
                hits += caches_update(*lst_it, calls.freqs_map/*.find(*lst_it)->second*/);
                lst_it++;
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H
