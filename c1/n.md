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

//void test_topk1(vector<int>& nums, int k)
//{
//	sort(nums.begin(),nums.end(),greater<int>());
//	return nums[k-1];
//}
//O(n*logn)*
//void test_topk2(vector<int>& nums, int k)
//{
//	priority_queue<int> manHeap(nums.begin(),nums.end());
//	while(--k)//pop k-1 times
//	{
//		maxHeap.pop();
//	}
//	return manHeap.top();
//}
//O(N+K*logN)*
//void test_topk3(vector<int>& nums, int k)
//{
//	priority_queue<int, vector<int>, greater<int>> minHeap(nums.begin(),nums.begin()+k);
//	for(size_t i = k;i<nums.size();++i)
//	{
//		if(nums[i]>minHeap.top())
//		{
//			minHeap.pop();
//			minHeap.push(nums[i]);
//		}
//	}
//	return minHeap.top();
//}
//O(K+(N-K)*logK) --> O(N*logK)
//the best : 3
REMEMBER: 
when using priority_queue :priority_queue<int, vector<int>, greater<int> > pq; ->greater<int> : type
when using sort : sort(nums.begin(),nums.end(), greater<int>() ); ->greater<int>() :anonymous object

2 functor
actually operator overloading.

e.g., 
less and greater  (using head file : <ulitity>)
how to use *less*? less(1,2). like a function but not.

maybe we need to overload the > or < operators on our own to use the functor.
e.g. Date struct.
But when we use Date*  *for example: priority_queue<int, vector<int> >
we need to create a new functor:

*struct LessPDate
{
	bool operator(const Date* d1,const Date* d2) const
	{
		return *d1 < *d2;
	}
}*
Then use the new functor.


---------------------------------------------------------Template-----------------------------------------------
template advanced:

non-type template parameter, only integer!!

for example:
 
can set default parameter:

template <class T,size_t N = 100>  //N -> non-type template parameter --> must be integer constant
class Mystack{
	public:
		void Push(const T& x)
		{}
	private:
		T _a[N];
		size_t _top;

};
int main()
{
	Mystack<int,100> st;
	st.Push(10);
}

template specialization:

when we use ObjLess to compare ,actually compared thing is the address.
SO we need to create a new overloading: 

template <class T>
bool ObjLess(const T& p1, const T& p2)
{
	return left < right;
}
int main()
{
	cout << ObjLess(1,2) << endl;
	Date* p1 = new Date(2000,1,1);
	Date* p2 = new Date(2010,1,1);

	cout << ObjLess(p1,p2) << endl;
}


new overloading:

bool ObjLess(const Date*& p1 ,const Date*& p2)
{
	return *left < *right;
}

then we can compare Date* type by the new overloading function*.
This is called *parameter matching*

way2: 

template<>
bool ObjLess<const Date*&>(const Date*& left, const Date*& right)
{
	return *left < *right;
}

this is called *"specialization"  / explicit instantiation.*

specialization - specialize for certain types.

template<>
struct Less<Date*>
{
	bool operator()(Date* x, Date* y) const {
		return *x < *y;
	}
}

This is the specialization.


