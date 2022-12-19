//red black tree

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

template<class K, class V>
struct RBTreeNode
{
    RBTreeNode<K,V>* _left;
    RBTreeNode<K,V>* _right;
    RBTreeNode<K,V>* _parent;
    pair<K,V> _kv;

    Colour _col;

    RBTreeNode(const pair<K,V>& kv )
        :_left(nullptr)
        ,_right(nullptr)
        ,_parent(nullptr)
        ,_col(RED)
        ,_kv(kv)
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
            _root->_col = BLACK;
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
                return false;
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

        return true;
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

    bool IsBalance(){
        if(_root && _root->_col == RED)
        {
            return false;
        }

        int benchMark = 0;
        Node* left = _root;
        while(left){
            if(left->_col == BLACK){
                ++benchMark;
            }
            left = left->_left;
        }
        
        int blackNum = 0;
        return _IsBalance(_root,benchMark,blackNum);
    }

    bool _IsBalance(Node* root, int benchMark, int blackNum){
        if(root == nullptr){
            if(blackNum != benchMark){
                cout << "blackNum != benchMark " << endl;
                return false;
            }
            return true;
        }

        if(root->_col == RED && root->_parent->_col == RED){
            cout << "continuous red nodes" << endl;
            return false;
        }
        if(root->_col == BLACK){
            ++blackNum;
        }
        return _IsBalance(root->_left,benchMark,blackNum) 
        && _IsBalance(root->_right,benchMark,blackNum);
    }

    int Height(){
        return _Height(_root);
    }
    int _Height(Node* root){
        if(root == nullptr)
            return 0;
        int leftHeight = _Height(root->_left);
        int rightHeight = _Height(root->_right);
        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }
private:
    Node* _root;
    //ROOT
};


void TestRBTree(){
    RBTree<int,int> t;
    int a[] = {4,2,6,1,3,4,15,7,16,14};
    for(auto e : a){
        t.Insert(make_pair(e,e));
    }
    t.InOrder();
}
void TestRBTree2(){
    RBTree<int, int> t;
    vector<int> v;
    srand(time(0));
    int N = 100;
    for(int i = 0 ; i < N; ++i){
        v.push_back(rand());
    }

    for(auto e : v){
        t.Insert(make_pair(e,e));
        if(!t.IsBalance()){
            cout << "Insert" << e << endl;
        }
    }

    t.InOrder();
    cout << t.IsBalance() << endl;
    cout << t.Height() << endl;
}
int main()
{
    TestRBTree2();
    return 0;
}