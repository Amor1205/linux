//red black tree

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

template<class K, class V>
struct RBTreeNode
{
    RBTreeNode<K,V>* _left;
    RBTreeNode<K,V>* _right;
    RBTreeNode<K,V>* _parent;
    pair<K,V> _kv;

    Colour _col;

    RBTreeNode(const pair<K,V> kv )
        :_kv(kv)
    {}
};

template<class K, class V>
struct RBTree
{
    typedef RBTreeNode<K,V> Node;
public:
    RBTree()
        :_root(nullptr)
    {}
    bool Insert(const pair<K,V> kv)
    {
        if(!_root){
            _root = new Node(kv);
            return true;
        }
        Node* parent = nullptr;
        Node* cur = _root;
        while(cur){
            if(cur->_kv.first < kv.first){
                parent = cur;
                cur = cur->_right;
            }
            else if(cur->_kv.first > kv.first){
                parent = cur;
                cur = cur->_left;
            }
            else{
                return flase;
            }
        }
        //cur is the position where we need to insert.
        cur = new Node(kv);
        if(parent->_kv.first < kv.first){
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
        // 1. uncle must is red.
        //
    }
private:
    Node* _root;
};