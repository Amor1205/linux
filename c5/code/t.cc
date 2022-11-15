//BInarySearchTree;
#pragma once
#include <iostream>
#include<string>
using namespace std;

namespace K{
template<class K>

//struct BinarySearchTreeNode{
struct BinarySearchTreeNode{
	BinarySearchTreeNode<K>* _left;
	BinarySearchTreeNode<K>* _right;
	K _key;

	BinarySearchTreeNode(const K& key)
		:_left(nullptr),_right(nullptr),_key(key)
	{}
};

template<class K>
class BSTree{

	typedef BinarySearchTreeNode<K> Node;
	//typedef BinarySearchTree<K> BSTree;
public:
	BSTree()
		:_root(nullptr)
	{}
	bool Insert(const K& key){
		if(_root == nullptr){
			_root = new Node(key);
			return true;
		}
		else{
			Node* parent = nullptr;
			Node* cur = _root;
			while(cur){
				if(cur->_key < key){
					parent = cur;
					cur = cur->_right;
				}
				else if(cur->_key > key){
					parent = cur;
					cur = cur->_left;
				}
				else{//already have the value node.
					return false;
				}
			}
			cur = new Node(key);
			if(parent->_key < key){
				parent->_right = cur;

			}else{
				parent->_left = cur;
			}
			return true;
		}
	}
	
	void InOrder(){
		_InOrder(_root);
	}

	void _InOrder(Node* root){
		if(root == nullptr){
			return ;
		}
		else{
			_InOrder(root->_left);
			std::cout << root->_key << " ";
			_InOrder(root->_right);
		}
	}
	
	bool Find(const K& key){
		Node* cur = _root;
		while(cur){
			if(cur->_key > key){
				cur = cur->_left;

			}
			else if (cur->_key < key){
				cur = cur->_right;
			}
			else{
				return true;
			}
		}
		return false;
	}
	
	bool Erase(const K& key){
			Node* cur = _root;
			Node* parent = nullptr;
			while(cur){
				if(cur->_key > key){
					parent = cur;
					cur = cur->_left;
				}
				else if(cur->_key < key){
					parent = cur ;
					cur = cur->_right;
				}
				else{
					//find.
					if(!cur->_left){
						if(!parent){
							_root = cur->_right;
						}
						if(cur == parent->_left){
							parent->_left = cur->_right;
							
						}
						else{
							parent->_right = cur->_right;
						}
						delete cur;
					}
					else if(!cur->_right){
						if(!parent){
							_root = cur->_left;
						}
						if(cur == parent->_left){
							parent->_left = cur->_left;
						}
						else{
							parent->_right = cur->_left;
						}
						delete cur;
					}
					else{
						//substitution method. 
						//find right tree -> leftmost node
						/*
						Node* current = cur;
						Node* current_par = parent;
						while(current->_left){
							current_par = current;
							current = current->_left;
						}
						current_par->_left = nullptr;
						current->_left = cur->_left;
						current->_right = cur->_right;
						if(parent){
							
							if(parent->_left == cur){
								parent->_left = current;
							}
							else{
								parent->_right = current;
							}
						}
						delete cur;
						*/
						
						Node* right_min = cur->_right;
						Node* right_min_par = cur;
						while(right_min->_left){
							right_min_par = right_min;
							right_min = right_min->_left;
						}
						cur->_key = right_min->_key;
						if(right_min_par->_left == right_min){
							right_min_par->_left = right_min->_right;
						}
						else {
							right_min_par->_right = right_min->_right;
						}
						delete right_min;


					}

					return true;
				}
			}
		return false;	
	}
	void get_root()const{
		std::cout << "_root:>"<<_root->_key << std::endl;
	}

	bool _InsertR(Node*& root,const K& key){
		if(!root){
			root = new Node(key);
			return true;
		}
		else{
			if(root->_key < key){
				return _InsertR(root->_right,key);
			}else if(root->key > key){
				return _InsertR(root->_left,key);
			}else{
				return false;
			}
		}
	}
	void InsertR(const K& key){
		_InsertR(_root,key);
	}

	Node* _FindR(Node* root, const K& key){
		if(root == nullptr){
			return nullptr;
		}else{
			if(root->_key > key){
				return _FindR(root->_left,key);
			}
			else if(root->_key < key){
				return _FindR(root->_right,key);
			}
			else{
				return root;
			}
		}
	}
	void FindR(const K& key){
		_FindR(_root,key);
	}

	bool _EraseR(Node* root, const K& key){
		if(!root){
			return false;
		}else{
			if(root->_key < key){
				return _EraseR(root->_right,key);
			}else if(root->key > key){
				return _EraseR(root->_left,key);
			}else{
				Node* del = root;
				if(!root->_left){
					root = root->_right;
				}else if(!root->_right){
					root = root->_left;
				}else{
					Node* right_min = root->_right;
					while(right_min->_left){
						right_min = right_min->_left;
					}
					swap(right_min->_key, root->_key);

					return _EraseR(root->_right, key);
				}
				delete del;
			}
		}
	}
	void EraseR(const K& key){
		_EraseR(_root, key);
	}

private:
	Node* _root;


};

class A{
	public:
		virtual void func(int val = 1){
			std::cout << "a->" << val << std::endl;
		}
		virtual void test(){
			func();
		}
};
class B: public A{
	public: 
		void func(int val = 0){
			std::cout << "B->" << val << std::endl;
		}
};

//comment: when virtual functions are rewritten, they inherit the virtual function interface of the parent function and rewrite the implementation of the virtual function.
//here, use <func> in derived class, <val> in parent class.

void test1(){
	BSTree<int> t;
	int a[] = {5,3,4,1,7,8,2,6,0,9,5,5,5,5,4,2};
	for(auto e : a){
		t.Insert(e);
	}
	t.get_root();
	t.InOrder();
	t.Erase(3);
	std::cout << std::endl;
	t.InOrder();
	t.get_root();
}
void test2(){
	B* p = new B;
	p->test();
}

}


namespace KV{
template<class K, class V>
//struct BinarySearchTreeNode{
struct BinarySearchTreeNode{
	BinarySearchTreeNode<K,V>* _left;
	BinarySearchTreeNode<K,V>* _right;
	K _key;
	V _value;
	BinarySearchTreeNode(const K& key,const V& value)
		:_left(nullptr),_right(nullptr),_key(key),_value(value)
	{}
};

template<class K,class V>
class BSTree{

	typedef BinarySearchTreeNode<K,V> Node;
	//typedef BinarySearchTree<K> BSTree;
public:
	BSTree()
		:_root(nullptr)
	{}
	bool Insert(const K& key,const V& value){
		if(_root == nullptr){
			_root = new Node(key,value);
			return true;
		}
		else{
			Node* parent = nullptr;
			Node* cur = _root;
			while(cur){
				if(cur->_key < key){
					parent = cur;
					cur = cur->_right;
				}
				else if(cur->_key > key){
					parent = cur;
					cur = cur->_left;
				}
				else{//already have the value node.
					return false;
				}
			}
			cur = new Node(key,value);
			if(parent->_key < key){
				parent->_right = cur;

			}else{
				parent->_left = cur;
			}
			return true;
		}
	}
	
	Node* Find(const K& key){
		Node* cur = _root;
		while(cur){
			if(cur->_key > key){
				cur = cur->_left;

			}
			else if (cur->_key < key){
				cur = cur->_right;
			}
			else{
				return cur;
			}
		}
		return nullptr;
	}
	
	bool Erase(const K& key){
			Node* cur = _root;
			Node* parent = nullptr;
			while(cur){
				if(cur->_key > key){
					parent = cur;
					cur = cur->_left;
				}
				else if(cur->_key < key){
					parent = cur ;
					cur = cur->_right;
				}
				else{
					//find.
					if(!cur->_left){
						if(!parent){
							_root = cur->_right;
						}
						if(cur == parent->_left){
							parent->_left = cur->_right;
							
						}
						else{
							parent->_right = cur->_right;
						}
						delete cur;
					}
					else if(!cur->_right){
						if(!parent){
							_root = cur->_left;
						}
						if(cur == parent->_left){
							parent->_left = cur->_left;
						}
						else{
							parent->_right = cur->_left;
						}
						delete cur;
					}
					else{
						//substitution method. 
						//find right tree -> leftmost node
						/*
						Node* current = cur;
						Node* current_par = parent;
						while(current->_left){
							current_par = current;
							current = current->_left;
						}
						current_par->_left = nullptr;
						current->_left = cur->_left;
						current->_right = cur->_right;
						if(parent){
							
							if(parent->_left == cur){
								parent->_left = current;
							}
							else{
								parent->_right = current;
							}
						}
						delete cur;
						*/
						
						Node* right_min = cur->_right;
						Node* right_min_par = cur;
						while(right_min->_left){
							right_min_par = right_min;
							right_min = right_min->_left;
						}
						cur->_key = right_min->_key;
						cur->_value = right_min->_value;
						if(right_min_par->_left == right_min){
							right_min_par->_left = right_min->_right;
						}
						else {
							right_min_par->_right = right_min->_right;
						}
						delete right_min;


					}

					return true;
				}
			}
		return false;	
	}
	void get_root()const{
		std::cout << "_root:>"<<_root->_key << std::endl;
	}

	void InOrder(){
		_InOrder(_root);
	}

	void _InOrder(Node* root){
		if(root == nullptr){
			return ;
		}
		else{
			_InOrder(root->_left);
			std::cout <<"key:" <<root->_key << " "<<"value:" <<root->_value << " ";
			_InOrder(root->_right);
		}
	}
private:
	Node* _root;


};
void TestBSTree1(){
	BSTree<std::string,std::string> dict;
	dict.Insert("sort","paixu");
	dict.Insert("left","zuobian");
	
	std::string str;
	while(std::cin >> str){
		BinarySearchTreeNode<std::string,std::string>* ret = dict.Find(str);
		if(ret){
			std::cout << "CHINESE:>" << ret->_value << std::endl;
		}else{
			std::cout << "NO thing can be found."<< std::endl;
		}
	}
}


void TestBSTree2(){
	string arr[] = {"key","value","name","name"};
	BSTree<string,int> countTree;
	for(auto& str : arr){
		auto ret = countTree.Find(str);
		if(ret != nullptr){
			ret->_value ++;
		}
		else{
			countTree.Insert(str,1);
		}
	}
	countTree.InOrder();
}
}//KV
/*void test1(){
	BSTree<int> t;
	int a[] = {5,3,4,1,7,8,2,6,0,9,5,5,5,5,4,2};
	for(auto e : a){
		t.Insert(e);
	}
	t.get_root();
	t.InOrder();
	t.Erase(3);
	std::cout << std::endl;
	t.InOrder();
	t.get_root();
}
void test2(){
	B* p = new B;
	p->test();
}
*/

int main(){
//	test1();
	//test2();
	KV::TestBSTree2();
	return 0;
}
