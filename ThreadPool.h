#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "GenericTask.h"
#include <ThreadSafeQueue.h>
#include <vector>
#include <functional>
#include <future>
#include <thread>
#include <sstream>
#include <atomic>
#include <iostream>
class ThreadPool {
    private:
        std::atomic<int> queIndex;
        unsigned int maxThreads;
        void workerThread(unsigned int iD);
        std::vector<ThreadSafeQueue<GenericTask>> queueTasks;
        std::vector<std::thread> workerThreads;
        void setUp();
        void deSetup();
        bool taskDistribute(const unsigned int& thrID);
        void notifyAll();
        void terminationMethod();
    public:
        ThreadPool();
        ThreadPool(const unsigned int& threadCount);
        template<typename functionType,typename ...Args>
        std::future<typename std::result_of<functionType(Args...)>::type>
             enqueueTask(functionType function, Args... arguments);
        ~ThreadPool();


};

template<typename functionType, typename ...Args>
std::future<typename std::result_of<functionType(Args...)>::type>
ThreadPool::enqueueTask(functionType function, Args... arguments)
{

    auto bind = std::bind(function,std::forward<Args>(arguments)...);
    typedef typename std::result_of<decltype(function)(Args...)>::type result_type;
    std::packaged_task<result_type()> packagedTask(std::move(bind));
    std::future<result_type> futureVal(packagedTask.get_future());
    //std::cout << typeid(function).name();
    GenericTask taskEnqueue(std::move(packagedTask));
    //std::cout << queIndex << std::endl;
    queIndex = (++queIndex)%maxThreads;
    this->queueTasks[queIndex].push(std::move(taskEnqueue));
    return   std::move(futureVal);

}

void ThreadPool::workerThread(unsigned int iD)
{
//    std::stringstream msg;
//    msg<<  "Thread " << iD << " has intialize" << std::endl;
//    std::cout<< msg.str();
    GenericTask taskToRun;
    while(true){
        //Get Task From Queue
//        msg = std::stringstream();
       this->queueTasks[iD].waitOrPop(taskToRun);
//        msg << "Thread " << iD << "is running a task" << std::endl;
//        std::cout << msg.str();
        taskToRun();
        for (unsigned int i = 1 ; i < this->maxThreads ; i++){
            if(this->taskDistribute((iD+i)%maxThreads)){
//                std::stringstream msg;
//                msg << "Thread"<< iD << "is Running  A Task Stealer"<<std::endl;
//                std::cout<< msg.str();
            }
        }
    }
}

void ThreadPool::setUp()
{
    queueTasks = std::vector<ThreadSafeQueue<GenericTask>>(maxThreads);
    this->queIndex = 0;
    for (unsigned int i = 0; i < maxThreads ; i++){
        std::thread workerThread_i= std::thread(&ThreadPool::workerThread,this,i);
        workerThreads.push_back(std::move(workerThread_i));
    }
}

bool ThreadPool::taskDistribute(const unsigned int &thrID)
{
    GenericTask taskToRun;
    if (this->queueTasks[thrID].tryPop(taskToRun)){

        taskToRun();
        return true;
    }
    else{
      return false;
    }
    
    
}

void ThreadPool::terminationMethod()
{
    for (int i = 0 ; i < this->queueTasks.size(); i++){
        //this->enqueueTask(&ThreadPool::terminationMethod,this);
    }
    exit(0);
    //std::terminate();

}

ThreadPool::ThreadPool()
{
    std::cout << "Yea";
    this->maxThreads = std::thread::hardware_concurrency();
    setUp();


}

ThreadPool::ThreadPool(const unsigned int &threadCount)
{
    this->maxThreads = threadCount;
    setUp();
}

ThreadPool::~ThreadPool()
{
    //std::cout << "Deconstruct ThreadPool";
    for (int i = 0 ; i < maxThreads ; i++){

        this->enqueueTask(&ThreadPool::terminationMethod,this);
         //std::cout << this->queIndex<< std::endl;
    }

    for (auto & thread: this->workerThreads){
        thread.join();

    }
}
#endif // THREADPOOL_H
