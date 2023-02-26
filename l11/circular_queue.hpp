#include<iostream>
#include<vector>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<unistd.h>
namespace ns_circular_queue{
    const int g_default_cap = 10;

    template<class T>
    class CircularQueue{
    private:
        std::vector<T> _circular_queue;
        int _capacity;
        sem_t _blank_sem; //producer uses
        sem_t _data_sem; //consumer uses
        int _c_pos = 0;
        int _p_pos = 0;
    public:
        CircularQueue(int cap = g_default_cap) :_circular_queue(cap), _capacity(cap){
            sem_init(&_blank_sem, 0, cap);
            sem_init(&_data_sem, 0, 0);
        }
        ~CircularQueue(){
            sem_destroy(&_blank_sem);
            sem_destroy(&_data_sem);
        }
    public:
        void Push(const T& in)
        {
            sem_wait(&_blank_sem);
            _circular_queue[_p_pos] = in;
            sem_post(&_data_sem);
            _p_pos ++;
            _p_pos %= _capacity;

        }
        void Pop(T* out)
        {
            sem_wait(&_data_sem);
            *out = _circular_queue[_c_pos];
            sem_post(&_blank_sem);
            _c_pos ++;
            _c_pos %= _capacity;
        }
    };
};