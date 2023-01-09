#pragma once
#include<vector>
#include<string>
#include<iostream>
#include<algorithm>
#include<queue>
#include<functional>
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
    int mid_element = (right - left)/2 + left;
    if(a[left] > a[mid_element])
    {
        if(a[mid_element] > a[right])
        {
            return mid_element;
        }
        else
        {
            if(a[left] > a[right])
            {
                return right;
            }
            else
            {
                return left;
            }
        }
    }   
    else // a[left] < a[mid_element]
    {
        if(a[left] > a[right])
        {
            return left;
        }
        else{
            if(a[right] > a[mid_element])
            {
                return mid_element;
            }
            else
            {
                return right;
            }
        }
    }
}
int partion(int*a, int left, int right)
{
    int mid_i = get_mid_index(a,left,right);
    int mid_num = a[mid_i];
    int left_i = left;
    int right_i = right;
    while(left_i >= right_i)
    {
        if(a[left_i] > mid_num)//换到最右边
        {
            
        }
    }
}
void quick_sort(int*a, int left, int right)
{
    if(right - left <= 1)
    {
        return;
    }
    int div = partion(a, left, right);
    quick_sort(a,left,div);
    quick_sort(a,div+1,right);

}//wrong