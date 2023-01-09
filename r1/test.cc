#include"sort.h"


int main()
{
    vector<int> v;
    int a[] = {5,3,2,5,7,9,11,1};
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
    quick_sort(a,0,7);
    // for(auto e : v)
    // {
    //     cout << e << " ";
    // }
    for(auto e : a)
    {
        cout << e << " ";
    }
}