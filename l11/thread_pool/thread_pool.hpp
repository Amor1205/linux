#pragma once

#include<time.h>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<string>
#include<queue>
#include<unistd.h>
#include<pthread.h>
#include"task.hpp"
namespace ns_threadpool{
    const int g_num = 5;
    template<class T>
    class ThreadPool{
    private:
        int _num; // no of threads
        std::queue<T> _task_queue; 
        pthread_mutex_t _mtx; // protect the queue 
        pthread_cond_t _cond; // let the threads wait when queue is empty
        static ThreadPool<T>* _inst;

    public:
        static ThreadPool<T>* GetInstance(){
            static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER
            if(_inst == nullptr)
            {
                pthread_mutex_lock(&lock);
                if(_inst == nullptr){
                    _inst = new ThreadPool<T>();
                    _inst->InitThreadPool();
                }
                pthread_mutex_unlock(&lock);
            }
            return _inst;
        }
        void Lock()
        {
            pthread_mutex_lock(&_mtx);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&_mtx);
        }
        bool IsEmpty()
        {
            return _task_queue.empty();    
        }
        void Wait()
        {
            pthread_cond_wait(&_cond,&_mtx);
        }    
        void WakeUp()
        {
            pthread_cond_signal(&_cond);
        }
    public:

        ~ThreadPool(){
            pthread_mutex_destroy(&_mtx);
            pthread_cond_destroy(&_cond);

        }   
        static void* Rountine(void* args){
            pthread_detach(pthread_self());
            //detach the main thread, the derived threads continue code below. The main thread excute the main()'s code
            ThreadPool<T>* tp = (ThreadPool<T>*)args;
            while(1){
                tp->Lock();
                while(tp->IsEmpty()){
                    //queue is empty, thread should rest(wait)
                    tp->Wait();
                }
                //handler task
                T t;
                tp->PopTask(&t);
                    
                //take the task and unlock .
                //dont waste lock resources
                tp->Unlock();
                t.Run();
                
                /*std::cout << pthread_self() << "Thread is running " << std::endl;
                sleep(1);*/

            }
        }
        void InitThreadPool()
        {
            pthread_t tid;
            for(int i = 0; i < _num; ++i)
            {
                pthread_create(&tid, nullptr, Rountine, (void*)this);

            }
        }  
        void PushTask(const T& in){
            Lock();
            _task_queue.push(in);
            Unlock();
            WakeUp();
        }  

    private:
        void PopTask(T* out){
            *out = _task_queue.front();
            _task_queue.pop();
        }
        ThreadPool(int num = g_num) :_num(num){
            pthread_mutex_init(&_mtx, nullptr);
            pthread_cond_init(&_cond, nullptr);
        }
        ThreadPool(const ThreadPool<T>& tp) = delete;
        ThreadPool<T> operator()(ThreadPool<T>& tp) = delete;
    };
    template<class T>
    ThreadPool<T>* ThreadPool<T>::_inst = nullptr;
};