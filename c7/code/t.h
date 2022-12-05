#pragma once
#include<queue>
#include<string>
#include<vector>
#include<functional>
#include<iostream>
#include<map>
#include<set>
#include<algorithm>
#include<assert.h>
using namespace std;
template<class K, class V>
struct  AVLTreeNode{
	AVLTreeNode<K,V>* _left;
	AVLTreeNode<K,V>* _right;
	AVLTreeNode<K,V>* _parent;
	pair<K,V> _kv;
	int _bf; //balance factor	
		 //
	AVLTreeNode(const  pair<K,V>& kv)
		:_left(nullptr),_right(nullptr),_parent(nullptr),_bf(0),_kv(kv)
	{

	}
};

template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;

public:
	AVLTree()
		:_root(nullptr)
	{}

	bool Insert(const pair<K,V>& kv){
		if(_root == nullptr){
			_root = new Node(kv);
			return true;
		}
		Node* parent = nullptr;
		Node* cur = _root;
		while(cur){
			if(cur->_kv.first < kv.first){
				parent = cur;
				cur = cur->_right;
			}else if(cur->_kv.first > kv.first){
				parent = cur;
				cur = cur->_left;
			}else{
				return false;
			}
		}

		cur = new Node(kv);
		if(parent->_kv.first < kv.first){
			parent->_right = cur;
			cur->_parent = parent;
		}
		else{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//control balance;

		//1.refresh BF; new Node to ancestors;
		//2.if BF is invalid, spin the balance tree;
		
		while(parent){
			if(cur == parent->_left){
				parent->_bf --;

			}else{
				parent->_bf ++;
			}
			//if parent->_bf == 0;    do not need to refresh ( -1/+1 -> 0 the height of subtree containing parent node unchanges. )
			//if parent->_bf == 1/-1; need to refresh (0 -> -1/+1, the height of subtree containing parent node changes : +1 )  MAYBE refresh to root , MAYBE refresh to some Node;
			//if parent->_bf == 2/-2; need to spin (-1 -> -2, +1 -> +2)
			//
			if(parent->_bf == 0){
				break;
			}
			else if(parent->_bf == 1 || parent->_bf == -1){
				//fresh up node;
				cur = parent;
				parent = parent->_parent;
			}else if(parent->_bf == 2 || parent->_bf == -2){
				//spin
				if(parent->_bf == -2 && cur->_bf == -1){
					RotateR(parent);
				}
				else if(parent->_bf == 2 && cur->_bf == 1){
					RotateL(parent);
				}
				else if(parent->_bf == -2 && cur->_bf == 1){
					RotateLR(parent);
				}
				else if(parent->_bf = 2 && cur->_bf == -1){
					RotateRL(parent);
				}
				break;
			}
			else{
				//HAVE PROBLEMS
				assert(false);
			}
		}
	} 

	void RotateR(Node* parent){
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* PParent = parent->_parent;
		parent->_left = subLR;
		if(subLR){
			subLR->_parent = parent;
		}
		subL->_right = parent;
		parent->_parent = subL;

		if(parent == _root){
			_root = subL;
			_root->_parent = nullptr;
		}
		else{
			subL->_parent = PParent;
			if(PParent){
				if(PParent->_left == parent){
					PParent->_left = subL;
				}
				else{
					PParent->_right = subL;
				}
			}
		}
			subL->_bf = 0;
			parent->_bf = 0;
	}
	void RotateL(Node* parent){
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* PParent = parent->_parent;

		parent->_right = subRL;
		if(subRL){
			subRL->_parent = parent;	
		}
		subR->_left = parent;
		parent->_parent = subR;

		if(parent == _root){
			_root = subR;
			_root->_parent = nullptr;
		}
		else{
			subR->_parent = PParent;
			if(PParent){
				if(PParent){
					if(PParent->_left == parent){
						PParent->_left = subR;
					}else{
						PParent->_right = subR;
					}
				}
			}
			subR->_bf = parent->_bf = 0;
		}
	}
	void RotateLR(Node* parent){
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		if(bf == 0){
			subL->_bf = subLR->_bf = parent->_bf = 0;
		}
		else if(bf == -1){
			parent->_bf = 1;
			subL->_bf = subLR->_bf = 0;
		}
		else if(bf == 1){
			subL->_bf = -1;
			subLR->_bf = parent->_bf = 0;
		}
		else{
			assert(false);
		}
	}
	void RotateRL(Node* parent){
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf; // 1/-1/0

		RotateR(parent->_right);
		RotateL(parent);

		if(bf == 0){
			subR->_bf = subRL->_bf = parent->_bf = 0;
		}
		else if(bf == 1){
			parent->_bf = -1;
			subR->_bf = subRL->_bf = 0;
		}
		else if(bf == -1){
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}else{
			assert(false);
		}
	}

	void InOrder(){
		_InOrder(_root);
	}
	void _InOrder(Node* root){
		if(root == nullptr)
			return;
		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second <<" :bf"<< root->_bf << endl;
		_InOrder(root->_right);
	}


	int Height(Node* root){
		if(root == nullptr){
			return 0;
		}
		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);

		return leftHeight > rightHeight ? leftHeight+1 : rightHeight+1;
	}

	bool IsBalance(){
		return _IsBalance(_root);
	}
	bool _IsBalance(Node* root){
		if(root == nullptr){
			return true;
		}

		//current Tree
		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);

		return abs(rightHeight-leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
		//
	}
private:
	Node* _root;
};
