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

        template <class RandomIt> page_calls_list_t(size_t sz,
                                                    RandomIt first, RandomIt last)
        : sz_(sz)
        {
            for (; first != last; ++first)
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

	unsigned call_list_pop_front(KeyT key) 
        {
            if (calls_map_.empty()) return sz_ + 1;
            auto cur_call = calls_map_.find(key);
	    if (cur_call == calls_map_.end()) return sz_ + 1;

            auto &call_list = cur_call->second;
            if (call_list.empty()) return sz_ + 1;

            auto next_call_it  = call_list.begin();
            unsigned next_call = *next_call_it;
            call_list.erase(next_call_it); 
            return next_call;
        }

        const std::vector<KeyT>& pages() const {return pages_;}
        const std::unordered_map<KeyT, std::vector<unsigned>>& calls_map() const {return calls_map_;}
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
        template <class CallsListT> bool caches_update(const KeyT &key, CallsListT &calls)    
        {
            auto hit = hash_.find(key);    
            unsigned next_call = calls.call_list_pop_front(key);      
	    assert(next_call <= calls.size());

            if (hit == hash_.end())
            {   
                if (full())
                {
                    auto further_incache_it = call_id_.begin();
                    assert(further_incache_it != call_id_.end());
                    if (next_call < further_incache_it->first)
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
        perfect_cache_t(size_t sz, PageCallT slow_get_page) 
        : sz_(sz), slow_get_page_(slow_get_page) {}

        int calculate_hits(page_calls_list_t<KeyT> &calls)
        {
            auto lst_it = calls.pages().begin();

            int hits = 0;
            for (size_t i = 0; i < calls.size(); i++, ++lst_it)
            {
                hits += caches_update(*lst_it, calls);
            }

            return hits;
        }
    };
    } // namespace caches

#endif // PERFECT_CACHING_H 
