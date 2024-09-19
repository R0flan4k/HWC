#ifndef PERFECT_CACHING_H
    #define PERFERCT_CACHING_H

    #include <map>
    #include <unordered_map>
    #include <list>

    namespace caches {

    template <typename KeyT> class page_calls_list_t {
        size_t sz_;
    
    public:
        std::unordered_map<KeyT, std::list<unsigned>> calls_map;
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

                auto hit = calls_map.find(cur_key);
                if (hit == calls_map.end())
                {
                    calls_map.emplace(cur_key, 0);
                }
                else
                {
                    hit->second.push_back(sz_ - sz);
                }
                sz--;
            }

            for (auto map_it = calls_map.begin(); map_it != calls_map.end(); map_it++)
                map_it->second.push_back(sz_);
        }
    };

    template <typename T, typename KeyT, typename PageCallT> class perfect_cache_t {
        size_t sz_;
        std::multimap<unsigned, KeyT, std::greater<unsigned>> call_id_; ///////

        using MapIt = typename std::multimap<unsigned, KeyT, std::greater<unsigned>>::iterator;
        struct map_entry_t {
            MapIt map_it;
            unsigned next_call_id = 0;
            T entry;
        };
        std::unordered_map<KeyT, map_entry_t> hash_;

        PageCallT slow_get_page_;

    public:
        bool full() const {return hash_.size() == sz_;}

    private:
        template <class CallsMapT> bool caches_update(const KeyT &key, CallsMapT &calls)
        {
            auto hit = hash_.find(key);
            auto &call_list = calls.find(key)->second;
            unsigned next_call = *(call_list.begin());  // Expect that we can see a future
            call_list.pop_front();                      // so calls.find() is always founded.

            if (hit == hash_.end())
            {
                if (full() && next_call < (call_id_.begin())->first)
                {
                    hash_.erase(call_id_.begin()->second);
                    call_id_.erase(call_id_.begin());
                    hash_.try_emplace(key, call_id_.emplace(next_call, key), next_call, slow_get_page_(key));
                }
                else if (!full())
                {
                    hash_.try_emplace(key, call_id_.emplace(next_call, key), next_call, slow_get_page_(key));
                }
                return false;
            }

            map_entry_t &elt = hit->second;
            call_id_.erase(elt.map_it);
            elt.map_it = call_id_.emplace(next_call, key);
            elt.next_call_id  = next_call;
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
                hits += caches_update(*lst_it, calls.calls_map/*.find(*lst_it)->second*/);
                lst_it++;
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H
