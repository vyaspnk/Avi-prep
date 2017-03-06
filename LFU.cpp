//
//  LFU.cpp
//  Avi Networks
//
//  Created by Vijetha Malla on 3/3/17.
//  Copyright © 2017 vyas. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
using namespace std;
template <typename KeyType, typename ValueType>
class LFUCache {
private:
    
    int capacity;
    typedef typename std::pair<KeyType, ValueType> CachePair;
    typedef typename std::list<CachePair> CacheList;
    typedef typename CacheList::iterator CacheList_iterator_t;
	typedef typename std::list<CacheList> FreqList;
    typedef typename FreqList::iterator freq_list_iterator_t;
//    typedef typename std::list<freq_list_iterator_t> FreqItorList;

    FreqList freq_list;
    unordered_map<KeyType, freq_list_iterator_t> LRUMap;
    
public:
    void set(KeyType k,ValueType v) {
        auto FL_it = LRUMap.find(k);
        if(FL_it == LRUMap.end()){ //Key not preset
            if(freq_list.size()==0) {
                
            }
        }
        
        
        Cache.push_front(make_pair(k,v)); //push at the begin() of the CacheList.
        if(it==LRUMap.end()) { //key already exists in the map.
            auto cache_it = it->second; //find iterator into the list
            Cache.erase(cache_it); //remove that duplicate from the list.
        }
        LRUMap[k] = Cache.begin(); //update the LRUMap with the new iterator
        
        //if cache capacity exceeded the evict least recently used
        if(LRUMap.size() > capacity) {
            auto cache_it = Cache.end(); //end() will have the lease recently used
            cache_it--;
            Cache.pop_back();// removes the last/end element.
            LRUMap.erase(cache_it->first);
        }
        
    }
    ValueType get(KeyType k,ValueType r) {
        auto f_it = LRUMap.find(k); //find list iterator for key
        if(f_it == LRUMap.end()) { //no iterator return r;
            return r;
        } else {
            ValueType v = f_it->second->second; //list iterator found. Get the Value at the list iterator
            Cache.splice(Cache.begin(), Cache, f_it->second); //
            LRUMap[k] = Cache.begin();
            return v;
        }
    }
    bool exists(KeyType &k) {
        return (LRUMap.find(k)!=LRUMap.end())?true:false;
    }
    LRUCache(int val) : capacity(val) {}
};

int main() {
    LRUCache<string, int> LRU_int(2);
    LRU_int.set("A", 10);
    LRU_int.set("B", 20);
    cout << "get(A) " << LRU_int.get("A",-1) << endl;
    LRU_int.set("C", 20);
    cout << "get(B) " << LRU_int.get("B",-1) << endl;
    LRU_int.set("B", 20);
    cout << "get(B) " << LRU_int.get("B",-1) << endl;
    cout << "get(A) " << LRU_int.get("A",-1) << endl;
    cout << "get(C) " << LRU_int.get("C",-1) << endl;
    
    return 0;
}
//    struct Comp {
//        inline bool operator()(CachePair &a,CachePair &b) {
//            return a.second > b.second;
//        }
//    };
//    priority_queue<CachePair,list<CachePair>,Comp()> freq;

