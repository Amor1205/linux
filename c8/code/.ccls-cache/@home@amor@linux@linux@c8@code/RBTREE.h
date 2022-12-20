//red black tree
#pragma once

#include<time.h>
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


enum Colour
{
    RED,
    BLACK
};

template<class T>
struct RBTreeNode
{
    RBTreeNode<T>* _left;
    RBTreeNode<T>* _right;
    RBTreeNode<T>* _parent;
    T _data;

    Colour _col;

    RBTreeNode(const T& data )
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_col(RED)
        ,_data(data)
    {}
};

template<class T, class Ref, class Ptr>
struct __RBTreeIterator
{
	typedef RBTreeNode<T> Node;
	typedef __RBTreeIterator<T,Ref,Ptr> Self;
	Node* _node;

	__RBTreeIterator(Node* node)
		:_node(node)
	{}

	Ref operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &_node->_data;
	}

	Self& operator++()
	{
		if(_node->_right)
		{
			Node* left = _node->_right;
			while(left->_left)
			{
				left = left->_left;
			}
			_node = left;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while(parent && cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}

			_node = parent;
		}

		return *this;
	}

	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}
		
	bool operator==(const Self& s) const
	{
		return _node == s._node;
	}

	Self& operator--()
	{
		if(_node->_left)
		{
			Node* right = _node->_left;
			while(right->_right)
			{
				right = right->_right;
			}

			_node = right;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while(parent && cur == parent->_left)
			{
				cur = parent;
				parent = parent->_parent;
			}

			_node = parent;
		}
		return *this;
	}
};

//set RBTree<K,K> 
//map RBTree<K,pair<K,V>> 
template<class K, class T, class KeyOfT>
struct RBTree
{
    typedef RBTreeNode<T> Node;
public:
    typedef __RBTreeIterator<T,T&,T*> iterator;
    typedef __RBTreeIterator<T,const T&,const T*> const_iterator;

    iterator begin()
    {
	    Node* left = _root;
	    while(left && left->_left){
		    left = left->_left;
	    }
	    return iterator(left);
    }
    iterator end()
    {
	return iterator(nullptr);
    }

    iterator Find(const K& key)
    {
	    Node* cur = _root;
	    KeyOfT kot;
	    while(cur)
	    {
		    if(kot(cur->_data) < key)
			    cur = cur->_right;
		    else if(kot(cur->_data > key))
			    cur = cur->_left;
		    else 
			    return iterator(cur);
	    }
	    return end();
    }

    RBTree()
        :_root(nullptr)
    {}
    ~RBTree()
    {
	Destroy(root);
	root = nullptr;
    }
    RBTree(const RBTree<K,V,KeyOfT>& t)
    {
	_root = Copy(t._root);
    }
    void Copy(Node* root)
    {
	    if(!root)
	    {
		return nullptr;
	    }

	    Node* newRoot = new Node(root->_data);
	    newRoot->_col = root->_col;

	    newRoot->left = Copy(root->_left);
	    newRoot->right = Copy(root->_right);
	    if(newRoot->_left)
		    newRoot->_left->_parent = newRoot;
	    if(newRoot->_right)
		    newRoot->_right->_parent = newRoot;

	    return newRoot;

    }
    void Destroy(Node* root)
    {
	if(!root)
		return;

	Destroy(root->_left);
	Destroy(root->_right);

	delete root;
    }

    RBTree<K, T, KeyOfT>& operator=(RBTree<K, T, KeyOfT> t)
    {
	    swap(_root,t._root);
	    return *this;
    }

    pair<iterator,bool> Insert(const T& data)
    {
        if(!_root){
            _root = new Node(data);
            _root->_col = BLACK;
            return make_pair(iterator(_root),true);
        }

	KeyOfT kot;
        Node* parent = nullptr;
        Node* cur = _root;
        while(cur){
            if(kot(cur->_data) < kot(data)){
                parent = cur;
                cur = cur->_right;
            }
            else if(kot(cur->_data) > kot(data)){
                parent = cur;
                cur = cur->_left;
            }
            else{
                return make_pair(iterator(cur),false);
            }
        }
        //cur is the position where we need to insert.
        cur = new Node(data);
	Node* newnode = cur;
        if(kot(parent->_data) < kot(data)){
            parent->_right = cur;
            cur->_parent = parent;
        }
        else{
            parent->_left = cur;
            cur->_parent = parent;
        }
        //insert.

        //suppose that new node is red.
        // if parent is red. grandfather must be black 
        // 1. uncle must be red.
        // p & u become black, g becomes r.
        // then make g be cur, adjust up.

        while(parent && parent->_col == RED){  //not root
            Node* grandfather = parent->_parent;
            if(parent == grandfather->_left){
                Node* uncle = grandfather->_right;
                //1.
                if(uncle && uncle->_col == RED){
                    //change colour && adjust up
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    cur = grandfather;
                    parent = cur->_parent;
                    
                }
                //2.
                else{ //non exist  or black
                    //uncle -: right side.
                    if(cur == parent->_left){
                        //right rotation.
                        RotateR(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    else{
                        //twice rotation
                        RotateL(parent);
                        RotateR(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    break;
                }
            }
            else{//parent == grandfather->_right
                Node* uncle = grandfather->_left;
                if(uncle && uncle->_col == RED){
                    //change colour && adjust up
                    parent->_col = uncle->_col = BLACK;
                    grandfather->_col = RED;
                    cur = grandfather;
                    parent = cur->_parent;
                    
                }
                else{
                    if(cur == parent->_right)
                    {
                        RotateL(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    else{
                        RotateR(parent);
                        RotateL(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                    }

                    break;

                }
            }
        }

        // 2. uncle non-exists or is black;
        // left side is higher than right side.
        // Rotation + colour change.
        //

        _root->_col = BLACK;//universal handling

        return make_pair(iterator(newnode),true);
    }

    void InOrder(){
		_InOrder(_root);
	}
	void _InOrder(Node* root){
		if(root == nullptr)
			return;
		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
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
		}
	}

private:
    Node* _root;
    //ROOT
};


