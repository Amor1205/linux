the meaning of binary tree: searching tree;
left child treenode < parent node < right child treenode;

Using it in searching in memory;

the worst situation : find height times (height = logN)  max O(N)
Balanced Search Binary Tree: AVL tree, red black tree;

can inline function be virtual function ?
before inline can add virtual, but the function cannot be inline.
inline function has no address.
But virtual function has address in virtual table.

static member function cannot be virtual function :
static function has no this pointer, so use the way of calling member function , you cannot access virtual function table.

constructors cannot be virtual functions.
No meaningful! the meaning of virtual functions is forming a polymorphism call, a polymorphism call need to search virtual functions in virtual table. 
BUT the virtual table pointer in object is initialized at the constructor initialization list stage.

DO REMEMBER:
When forming a polymorphism, calling a virtual function needs to call it in virtual function table.if not forming a polymorphism, cannot call it in virtual table.

Object calls normal function or virtual function, 
IF forming a polymorphism, -> calling normal function is faster, because calling a virtual function needing to search the address in virtual table.
But if not forming, determine the address of a function at compile time. the virtual function is as fast as normal function.


the usage of searchtree:
1.search; -> key model
2.sorting and deduplication -> key + value model

key model : search -> for example, store student numbers.
k/v model : find one value by another.


