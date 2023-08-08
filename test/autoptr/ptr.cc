#include <iostream>
#include <mutex>
namespace amor{
    template<class T>
    class SmartPtr{
        private:
            T* _ptr;
        public:
            SmartPtr(T* ptr = nullptr) :_ptr(ptr) {}
            ~SmartPtr() {
                if (_ptr != nullptr)
                    delete _ptr;
            }
            T& operator*() {return *_ptr;}
            T* operator->() {return _ptr;}
    };
    template<class T>
    class SharedPtr{
        private:
            T* _ptr;
            int* _pcount;
            std::mutex* _pmutex;
        private:
            void Release() {
                _pmutex->lock();
                bool flag = false;
                if (--(*_pcount) == 0) {
                    delete _ptr;
                    delete _pcount;
                    flag = true;
                }
                _pmutex->unlock(); 
                if (flag == true) {
                    delete _pmutex;
                }
            }
            void AddRef() {
                _pmutex->lock();
                (*_pcount)++;
                _pmutex->unlock();
            }
        public:
            SharedPtr(T* ptr = nullptr) :_ptr(ptr),_pcount(new int(1)),_pmutex(new std::mutex) {}
            ~SharedPtr() {
                std::cout << "~SharedPtr(): "<< *_pcount  << std::endl;
                Release();
            }
            SharedPtr(const SharedPtr<T>& sp) 
                :_ptr(sp._ptr),_pcount(sp._pcount),_pmutex(sp._pmutex)
            {
                AddRef();
            }
            SharedPtr<T>& operator=(const SharedPtr<T>& sp) {
                if (_ptr == sp._ptr) {
                    return  *this; 
                }
                //如果不是
                Release();
                _ptr = sp._ptr;
                _pcount = sp._pcount;
                _pmutex = sp._pmutex;
                AddRef();
                return *this;
            }
            T& operator*() {
                return *_ptr;
            }
            T* operator->() {
                return _ptr;
            }
    };

    class Test{
        public:
        public:
            Test() {}
            ~Test() {std::cout << "~Test()" << std::endl;}
    };
    struct ListNode {
        public:
            int data;
            SharedPtr<ListNode> prev;
            SharedPtr<ListNode> next;
        public:
            ~ListNode() {std::cout << "~ListNode()" << std::endl;}
    };
};

int main() {
    /*Test1 : autoptr*/
    // auto autoptr = amor::SmartPtr<amor::Test>(new amor::Test());
    /*Test2: SharedPtr*/
    // auto sp1 = amor::SharedPtr<amor::Test>(new amor::Test());
    // auto sp2 = sp1;
    // auto sp3(sp2);
    // auto sp4(sp3);
    /*Test3: 循环引用*/
    amor::SharedPtr<amor::ListNode> node1(new amor::ListNode);
    amor::SharedPtr<amor::ListNode> node2(new amor::ListNode);
    node1->next = node2;
    node2->prev = node1;
    return 0;
}