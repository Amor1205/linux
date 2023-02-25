#pragma once

#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<cstdlib>
#include<queue>

using namespace std;
namespace ns_blockingQueue{
    const int default_capacity = 5;
    template<class T>
    class BlockingQueue
    {
    public:
        BlockingQueue(int cap = default_capacity)
            :_capacity(cap)
        {
            pthread_mutex_init(&_mtx, nullptr);
            pthread_cond_init(&_isEmpty, nullptr);
            pthread_cond_init(&_isFull, nullptr);

        }
        ~BlockingQueue(){
            pthread_mutex_destroy(&_mtx);
            pthread_cond_destroy(&_isEmpty);
            pthread_cond_destroy(&_isFull);
        }
        //const & : input type parameters
        // * : output type parameters
        // & : input & output parameters
        //push the data
        void Push(const T& in)
        {
            Lock();
            // below : critical Zone
            if(IsFull())
            {
                // when waiting, it has the lock.
                ProducerWait();
            }
            _bq.push(in);

            //reminder
            RemindConsumers(); // before or after is OK
            UnLock();

            
        }
        //pull the data
        void Pop(T* out)
        {
            Lock();
            if(IsEmpty())
            {
                ConsumerWait();
            }
            *out = _bq.front();
            _bq.pop();

            //reminder
            RemindProducers();
            UnLock();
        }
        void ProducerWait()
        {
            pthread_cond_wait(&_isEmpty, &_mtx);
        }
        void ConsumerWait()
        {
            pthread_cond_wait(&_isFull, &_mtx);
        }
    private:
        bool IsFull()
        {
            return _bq.size() == _capacity;
        }
        bool IsEmpty()
        {
            return _bq.size() == 0;
        }
        void Lock()
        {
            pthread_mutex_lock(&_mtx);
        }
        void UnLock()
        {
            pthread_mutex_unlock(&_mtx);
        }
            
        void RemindConsumers()
        {
            pthread_cond_signal(&_isFull);
        }
        void RemindProducers()
        {
            pthread_cond_signal(&_isEmpty);
        }
    private:
        std::queue<T> _bq;// blocking queue
        int _capacity = 10; // the capacity of bq
        pthread_mutex_t _mtx; // protect critical resources
        // 1. when producer is full, do not produce anymore.(do not complete for mutex). Let consumer consume.
        // 2. When consumer is empty, do not consume.(complete for mutex). 
        //    Let producer produce.
        pthread_cond_t _isFull; // _bq is full, consumer wait for cond
        pthread_cond_t _isEmpty; // _bq is empty, producer wait for cond
    };

};