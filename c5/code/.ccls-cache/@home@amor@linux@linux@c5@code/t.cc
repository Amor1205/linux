//BInarySearchTree;
#pragma once
#include <iostream>
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
		if(!Find(key)){
			return false;
		}
		else{
			Node* cur = _root;
			while(cur != ){

			}
		}

	}
private:
	Node* _root;


};

void test1(){
	BSTree<int> t;
	int a[] = {5,3,4,1,7,8,2,6,0,9,5,5,5,5,4,2};
	for(auto e : a){
		t.Insert(e);
	}
	t.InOrder();
}


int main(){
	test1();
	return 0;
}
