#include"t.h"

void test_AVL1(){
	AVLTree<int,int> t;
	int a[] = {5,4,3,2,1,0};
	for(auto e : a){
		t.Insert(make_pair(e,e));
	}
	t.InOrder();
	
	cout << t.IsBalance() << endl;
}

int main(){
	test_AVL1();
	return 0;
}
