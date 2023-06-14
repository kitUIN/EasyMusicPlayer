#pragma once

#include"Pair.hpp"
#include"Tree.hpp"

template<class Key, class Val>
class HashMap :private Tree<Pair<Key, Val>>
{
public:
    int size(void)
    {
        return Tree<Pair<Key, Val>>::size();
    }
    void clear(void)
    {
        Tree<Pair<Key, Val>>::clear();
    }
    bool empty(void)
    {
        return Tree<Pair<Key, Val>>::empty();
    }
    bool find(Key key)
    {
        return Tree<Pair<Key, Val>>::find(Pair<Key, Val>(key, Val()));
    }
    void erase(Key key)
    {
        Tree<Pair<Key, Val>>::erase(Pair<Key, Val>(key, Val()));
    }
    void insert(Key key, Val val)
    {
        Tree<Pair<Key, Val>>::insert(Pair<Key, Val>(key, val));
    }
    Val& operator()(int rank)
    {
        return Tree<Pair<Key, Val>>::getvalbyrank(rank).value();
    }
    Val& operator[](Key key)
    {
        Pair<Key, Val>pair(key);
        int rank = Tree<Pair<Key, Val>>::getrankbyval(pair);
        return Tree<Pair<Key, Val>>::getvalbyrank(rank).value();
    }
    void getall(std::vector<Val>& lib)
    {
        std::vector<Pair<Key, Val>>_lib;
        Tree<Pair<Key, Val>>::getall(_lib);
        for (int i = 0; i < _lib.size(); ++i)
        {
            lib.push_back(_lib[i].value());
        }
    }
};
