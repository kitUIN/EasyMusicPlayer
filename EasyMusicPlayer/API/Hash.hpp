#pragma once

#include<string>
using std::string;

typedef unsigned long long lnt;

const lnt Base1 = static_cast<lnt>(1e9 + 7);
const lnt Base2 = 19260817;
const lnt MOD = 998244353;

int Hash(string str)
{
    lnt hash1 = 0;
    lnt hash2 = 0;
    for (int i = 0; i < str.size(); ++i)
    {
        hash1 = (hash1 * Base1 + str[i]) % MOD;
        hash2 = (hash2 * Base2 + str[i]) % MOD;
    }
    return int(hash1 * hash2);
}
