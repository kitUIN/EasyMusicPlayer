#pragma once

#include<vector>

template<class Type>
bool Compare(const Type& a, const Type& b)
{
    return a < b;
}
template<class Type>
void MergeSort(int l, int r, std::vector<Type>& lib, std::vector<Type>& temp, bool (*cmp)(const Type&, const Type&))
{
    if (l < r)
    {
        int i = l;
        int mid = (l + r) >> 1;
        int p = l, q = mid + 1;
        MergeSort<Type>(l, mid + 0, lib, temp, cmp);
        MergeSort<Type>(mid + 1, r, lib, temp, cmp);
        while (p <= mid && q <= r)
        {
            if (cmp(lib[p], lib[q]))
            {
                temp[i++] = lib[p++];
            }
            else
            {
                temp[i++] = lib[q++];
            }
        }
        while (p <= mid)temp[i++] = lib[p++];
        while (q <= r)	temp[i++] = lib[q++];
        for (i = l; i <= r; i++)lib[i] = temp[i];
    }
}
template<class Type>
void Sort(std::vector<Type>& lib, bool (*cmp)(const Type&, const Type&) = Compare)
{
    std::vector<Type>temp(lib.size());
    MergeSort<Type>(0, static_cast<int>(lib.size()) - 1, lib, temp, cmp);
};
