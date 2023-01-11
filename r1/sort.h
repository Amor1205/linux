#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
#include<stack>
using namespace std;

//direct insertion sort
void direct_insert_sort(vector<int>& v)
{
    for(int i = 0;i < v.size()-1; ++i)
    {
        int end = i;
        int tmp = v[end+1];
        while(end >= 0)
        {
            if(v[end] > tmp)
            {
                v[end + 1] = v[end];
                --end;
            }
            else
            {
                break;
            }
        }
        v[end+1] = tmp;
    }
}
void shell_sort(vector<int>& v)
{
    int gap = v.size();
    while(gap > 1)
    {
        gap = gap / 3 + 1;
        cout << "gap : " << gap << endl;
        for(int i = 0; i < v.size()-gap; ++i)
        {
            int end = i;
            int tmp = v[end + gap];
            while(end >= 0)
            {
                if(v[end] > tmp)
                {
                    v[end + gap] = v[end];
                    end -= gap;
                }
                else
                {
                    break;
                }
            }
            v[end + gap] = tmp;
        }
    }

}

void direct_selection_sort(vector<int>& v)
{
    auto it = v.begin();
    for(int i = 0; i < v.size()-1; ++i)
    {
        auto pmin = min_element(it,v.end());
        if(v[i] != *pmin)
        {
            swap(v[i],*pmin);
        }
        ++it;
    }
}

void heap_sort(vector<int>& v)
{
    priority_queue<int,vector<int>, greater<int> > pq;
    for(auto e : v)
    {
        pq.push(e);
    }
    for(int i = 0; i <= v.size()-1; ++i)
    {
        if(v[i] != pq.top())
        {
            v[i] = pq.top();
        }
        pq.pop();
    }
}

void Swap(int* p1, int* p2)
{
    if(p1 == p2)
    {
        return;
    }

    int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void adjust_down(int* a,int n, int parent)
{
    int min_child = parent*2 + 1;
    while(min_child < n)
    {
        if(min_child+1 < n && a[min_child+1] < a[min_child])
        {
            ++min_child;
        }
        if(a[min_child] < a[parent])
        {
            Swap(&a[min_child], &a[parent]);
            parent = min_child;
            min_child = parent * 2 + 1;
        }
        else
        {
            break;
        }
    }
}

void c_heap_sort(int* a, int n)
{
    //form heap
    for(int i = (n - 1 - 1)/2; i >= 0; --i)
    {
        adjust_down(a,n,i);
    }
    //choose number
    int i = 1;
    while(i < n)
    {
        Swap(&a[0], &a[n-i]);
        adjust_down(a,n-i,0);
        ++i;
    }
}

void bubble_sort(int*a, int n)
{
    // 趟数，一趟可以选出来一个最大值，最小值
    // n/2就可以选出来所有的。
    // 记录走到哪了 j
    // int times = 0;
    // while(times < n - 1)
    // {
    //     for(int j = 0; j < n - 1; ++j)
    //     {
    //         if(a[j] > a[j+1])
    //         {
    //             Swap(&a[j], &a[j+1]);
    //         }
    //     }
    //     ++times;
    // }

    for(int j = 0; j < n; ++j)
    {
        int if_swapped = 0;
        for(int i = 0; i < n - j; ++i)
        {
            if(a[i - 1] > a[i])
            {
                Swap(&a[i-1], &a[i]);
                if_swapped = 1;
            }
        }
        if(if_swapped == 0)
        {
            break;
        }
    }
}
int get_mid_index(int*a,int left,int right)
{
    int mid = (right - left)/2 + left;
    if(a[left] > a[mid])
    {
        if(a[mid] > a[right])
        {
            return mid;
        }
        else if(a[left] > a[right])
            {
                return right;
            }
        else
            {
                return left;
            }
    }   
    else // a[left] <= a[mid]
    {
        if(a[left] > a[right])
        {
            return left;
        }
        else if(a[right] > a[mid])
        {
                return mid;
        }
        else
        {
            return right;
        }
    }
}
int part_sort_hoare(int*a, int left, int right)
{
    int mid_i = get_mid_index(a,left,right);
    Swap(&a[left],&a[mid_i]);

    int key_i = left;
    while(left < right)
    {
        while(left < right && a[right] >= a[key_i])
        {
            --right;
        }
        while(left < right && a[left] <= a[key_i])
        {
            ++left;
        }
        if(left < right)
        {
            Swap(&a[left], &a[right]);
        }
    }
    int meet_i = left;
    Swap(&a[meet_i], &a[key_i]);
    return meet_i;
}

int part_sort_hole(int*a, int left, int right)
{
    int mid = get_mid_index(a,left,right);
    Swap(&a[left],&a[mid]);

    int key = a[left];
    int hole = left;
    while(left < right)
    {
        while(left < right && a[right] >= key)
        {
            --right;
        }
        a[hole] = a[right];
        hole = right;

        while(left < right && a[left] <= key)
        {
            ++left;
        }
        a[hole] = a[left];
        hole = left;
    }

    a[hole] = key;
    return hole;
}

int part_sort_pointer(int*a ,int left, int right)
{
    int mid = get_mid_index(a,left,right);
    Swap(&a[left],&a[mid]);
    int key_i = left;
    int prev = left;
    int cur = left+1;

    while(cur <= right)
    {
        if(a[cur] < a[key_i] && ++prev != cur)
        {
            Swap(&a[cur], &a[prev]);
        }
        ++cur;
    }
    Swap(&a[key_i], &a[prev]);
    return prev;
}
void quick_sort(int*a, int left, int right)
{
    if(left >= right)
    {
        return;
    }
    int div = part_sort_pointer(a, left, right);
    quick_sort(a,left,div-1);
    quick_sort(a,div+1,right);

}

void quick_sort_nonR(int* a, int left, int right)
{
    stack<int> st;
    st.push(left);
    st.push(right);
    while(!st.empty())
    {
        int end = st.top();
        st.pop();
        int begin = st.top();
        st.pop();

        int key_i = part_sort_pointer(a,begin,end);

        if(key_i + 1 > right)
        {
            st.push(key_i+1);
            st.push(right);
        }
        if(left < key_i - 1)
        {
            st.push(left);
            st.push(key_i - 1);
        }
    }
}

void _merge_sort(int* a, int begin, int end, int* tmp)
{
    if(begin >= end)
    {
        return;
    }
    int mid = begin + (end - begin) / 2;
    _merge_sort(a, begin, mid, tmp);
    _merge_sort(a, mid + 1, end, tmp);

    int begin1 = begin, end1 = mid;
    int begin2 = mid + 1, end2 = end;
    int i = begin;
    while(begin1 <= end1 && begin2 <= end2)
    {
        if(a[begin1] <= a[begin2])
        {
            tmp[i++] = a[begin1++];
        }
        else
        {
            tmp[i++] = a[begin2++];
        }
    }

    while(begin1 <= end1)
    {
        tmp[i++] = a[begin1++];
    }
    while(begin2 <= end2)
    {
        tmp[i++] = a[begin2++];
    }

    memcpy(a + begin, tmp+begin, (end - begin + 1) * sizeof(int));

}
void merge_sort(int* a, int n)
{
    int* tmp = (int*) malloc(sizeof(int) * n);
    if(tmp == nullptr)
    {
        perror("malloc failed");
        return;
    }
    _merge_sort(a, 0, n-1, tmp);
    free(tmp);
    tmp = nullptr;
}

void merge_sort_nonR(int*a ,int n)
{
    int* tmp = (int*) malloc(sizeof(int) * n);
    if(tmp == nullptr)
    {
        perror("malloc failed");
        return;
    }
    //_merge_sort(a, 0, n-1, tmp);
    int gap = 1;
    while(gap < n)
    {
        for(int j = 0; j < n; j += 2*gap)
        {
            int begin1 = j, end1 = j + gap - 1;
            int begin2 = j + gap, end2 = j + 2*gap - 1;
            if(end1 >= n || begin2 >= n)
            {
                cout << "end1 >= n || begin2 >= n : WRONG RANGE";
                break;
            }
            if(end2 >= n)
            {
                cout << "end2 >= n : HAVE FIXED IT";
                end2 = n-1;
            }
            int i = j;
            while(begin1 <= end1 && begin2 <= end2)
            {
                if(a[begin1] <= a[begin2])
                {
                    tmp[i++] = a[begin1++];
                }
                else
                {
                    tmp[i++] = a[begin2++];
                }
            }
            while(begin1 <= end1)
            {
                tmp[i++] = a[begin1++];
            }
            while(begin2 <= end2)
            {
                tmp[i++] = a[begin2++];
            }
            memcpy(a+j, tmp+j, (end2 - j + 1) * sizeof(int));
        }
        gap *= 2;
    }
    free(tmp);
    tmp = nullptr;    
}

int* count_sort(int* a, int n)
{
    int max = a[0];
    int min = a[0];
    for(int i = 0; i < n; ++i)
    {
        if(max < a[i])
        {
            max = a[i];
        }
        if(min > a[i])
        {
            min = a[i];
        }
    }
    int range = max - min + 1;
    int* tmp = (int*) malloc(sizeof(int) * range);
    //check and memset

    memset(tmp, 0, sizeof(int) * range);

    for(int i = 0; i < n; ++i)
    {
        tmp[a[i] - min]++;
    }
    // for(int i = 0; i < range - 1; ++i)
    // {
    //     tmp[i+1] += tmp[i];
    // }

    // int* new_array = (int*) malloc(sizeof(int) * n);
    // for(int i = n - 1; i >= 0; i--)
    // {
    //     new_array[tmp[a[i] - min] - 1] = a[i];
    //     -- tmp[a[i] - min];
    // }   
    
    
    // free (tmp);
    // tmp = nullptr;
    // return new_array;

    int j = 0;
    for(int i = 0; i < range; ++i)
    {
        while(tmp[i]--)
        {
            a[j] = i + min;
            ++j;
        }
    }
    return a;
}