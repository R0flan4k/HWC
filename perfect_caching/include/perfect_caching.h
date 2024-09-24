#ifndef PERFECT_CACHING_H
    #define PERFERCT_CACHING_H

    #include <map>
    #include <unordered_map>
    #include <vector>
    #include <cassert>

    namespace caches {

    template <typename KeyT> class page_calls_list_t {
        size_t sz_;
        std::unordered_map<KeyT, std::vector<unsigned>> calls_map_;
        std::vector<KeyT> pages_;
    
    public:
        size_t size() const {return sz_;}

        template <class RandomIt> explicit page_calls_list_t(size_t sz,
                                                             RandomIt first, RandomIt last)
        : sz_(sz)
        {
            for (; first != last; first++)
            {
                KeyT cur_key = *first;
                pages_.push_back(cur_key);

                auto hit = calls_map_.find(cur_key);
                if (hit == calls_map_.end())
                {
                    calls_map_.try_emplace(cur_key);
                }
                else
                {
                    hit->second.push_back(sz_ - sz);
                }
                sz--;
            }

            for (auto map_it = calls_map_.begin(); map_it != calls_map_.end(); map_it++)
                map_it->second.push_back(sz_);
        }

        const std::vector<KeyT>& pages() const {return pages_;}
        std::unordered_map<KeyT, std::vector<unsigned>>& calls_map() {return calls_map_;}
    };

    template <typename KeyT, typename T, typename PageCallT> class perfect_cache_t {
        size_t sz_;
        std::map<unsigned, KeyT, std::greater<unsigned>> call_id_;

        using MapIt = typename std::map<unsigned, KeyT, std::greater<unsigned>>::iterator;
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
            assert(!calls.empty());
            assert(calls.find(key) != calls.end());

            auto hit = hash_.find(key);
            auto &call_list = calls.find(key)->second;
            assert(calls.find(key) != calls.end());
            assert(!call_list.empty());

            unsigned next_call = *(call_list.begin());  // Expect that we can see a future
            call_list.erase(call_list.begin());         // so calls.find() is always found.

            if (hit == hash_.end())
            {   
                if (full())
                {
                    if (next_call < (call_id_.begin())->first)
                    {
                        hash_.erase(call_id_.begin()->second);
                        call_id_.erase(call_id_.begin());
                    }
                    else 
                        return false;
                }
                hash_.try_emplace(key, call_id_.emplace(next_call, key).first, next_call, slow_get_page_(key));
                return false;
            }

            map_entry_t &elt = hit->second;
            call_id_.erase(elt.map_it);
            elt.map_it = call_id_.emplace(next_call, key).first;
            elt.next_call_id  = next_call;
            return true;
        }

    public:
        explicit perfect_cache_t(size_t sz, PageCallT slow_get_page) 
        : sz_(sz), slow_get_page_(slow_get_page) {}

        int calculate_hits(page_calls_list_t<KeyT> &calls)
        {
            auto lst_it = calls.pages().begin();

            int hits = 0;
            for (size_t i = 0; i < calls.size(); i++)
            {
                hits += caches_update(*lst_it, calls.calls_map());
                lst_it++;
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H 