priority_queue:container adaptors
using the head file : <queue>

template <class T,class Container = vector<T>>
class Compare = less<typename Container::value_type> > class priority_queue

context is similar to a heap.
need randomIterator.

push
pop
top(heap_top)
empty
size

[example]
amor@amor-win:~/linux/linux/c1$ make
g++ -o plist plist.cpp
amor@amor-win:~/linux/linux/c1$ ./plist
7
5
4
3
2
1

SO:
by default : the biggest one has the highest priority.
And if you want to change the smallest one into highest: use head file <functional> and do this:
creating pq: -> priority_queue<int, vector<int>, greater<int> > pq;


Application:
1. topK
if we only use sort,  O(N*logN)*

REMEMBER: 
when using priority_queue :priority_queue<int, vector<int>, greater<int> > pq; ->greater<int> : type
when using sort : sort(nums.begin(),nums.end(), greater<int>() ); ->greater<int>() :anonymous object


 
