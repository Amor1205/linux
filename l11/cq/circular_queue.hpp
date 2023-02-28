#include<iostream>
#include<vector>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
#include<unistd.h>
#include<string>
#include"task.hpp"
using namespace ns_task;
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
        pthread_mutex_t _c_mtx;
        pthread_mutex_t _p_mtx;
    public:
        CircularQueue(int cap = g_default_cap) :_circular_queue(cap), _capacity(cap){
            sem_init(&_blank_sem, 0, cap);
            sem_init(&_data_sem, 0, 0);
            pthread_mutex_init(&_c_mtx, nullptr);
            pthread_mutex_init(&_p_mtx, nullptr);
        }
        ~CircularQueue(){
            sem_destroy(&_blank_sem);
            sem_destroy(&_data_sem);
            pthread_mutex_destroy(&_c_mtx);
            pthread_mutex_destroy(&_p_mtx);

        }
    public:
        void Push(const T& in)
        {
            sem_wait(&_blank_sem);
            pthread_mutex_lock(&_p_mtx);
            _circular_queue[_p_pos] = in;
            _p_pos ++;
            _p_pos %= _capacity;
            pthread_mutex_unlock(&_p_mtx);
            sem_post(&_data_sem);

        }
        void Pop(T* out)
        {
            sem_wait(&_data_sem);
            pthread_mutex_lock(&_c_mtx);
            *out = _circular_queue[_c_pos];
            _c_pos ++;
            _c_pos %= _capacity;
            pthread_mutex_unlock(&_c_mtx);
            sem_post(&_blank_sem);
            //comment
        }
    };
};