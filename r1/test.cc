#include"sort.h"


int main()
{
    vector<int> v;
    int a[] = {5,3,2,1,5,7,9,11};
    for(auto e : a)
    {
        v.push_back(e);
    }
    //direct_insert_sort(v);
    //shell_sort(v);
    //direct_selection_sort(v);
    // heap_sort(v);
    //c_heap_sort(a,8);
    //bubble_sort(a,8);
    //quick_sort(a,0,7);
    //quick_sort_nonR(a,0,7);
    // merge_sort(a,8);
    //merge_sort_nonR(a,8);
    int* new_a = count_sort(a,8);
    // for(auto e : v)
    // {
    //     cout << e << " ";
    // }
    for(int i = 0; i < 8; ++i)
    {
        cout << new_a[i] << " ";
    }

    // for(auto e : a)
    // {
    //     cout << e << " ";
    // }
}