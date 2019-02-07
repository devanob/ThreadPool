#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H


//INCLUDES
#include <mutex>
#include <memory>
#include <queue>
#include <condition_variable>

template <typename Type>
class ThreadSafeQueue
{
    public:
        ThreadSafeQueue() = default;
        ThreadSafeQueue(ThreadSafeQueue const& otherQueue);
        //Push new items
        bool push(const Type& newItem);
        bool push(Type&& newItem);
        //Wait Or Pop A new iteam
        void waitOrPop(Type &item);
        std::unique_ptr<Type> waitOrPop();
        //Non-Block Pop
        bool tryPop(Type &item);
        std::unique_ptr<Type> tryPop();
        //Check if empty 
        bool empty() const;
        //notift
        void notify();
        ~ThreadSafeQueue();
        void clear();
        
    private:
        mutable std::mutex lockQueue;
        std::queue<Type> dataQueue;
        std::condition_variable data_cond_var;
        
};

template<typename Type>
ThreadSafeQueue<Type>::ThreadSafeQueue(const ThreadSafeQueue &otherQueue)
{
    std::unique_lock<std::mutex> queuLock(otherQueue.lockQueue);
    this->dataQueue = otherQueue.dataQueue;
}

template<typename Type>
bool ThreadSafeQueue<Type>::push(const Type& newItem)
{
   std::lock_guard<std::mutex> queuLock(this->lockQueue);
   try {
    this->dataQueue.push(newItem);
   }
   catch(std::exception& expt){
       return false;
   }
   this->data_cond_var.notify_one();
   return true;
}

template<typename Type>
bool ThreadSafeQueue<Type>::push(Type &&newItem)
{
    std::lock_guard<std::mutex> queuLock(this->lockQueue);
    try {
        this->dataQueue.push(std::move(newItem));
    }
    catch(std::exception &expt){
        return false;
    }
    this->data_cond_var.notify_one();
    return true;
}

template<typename Type>
void ThreadSafeQueue<Type>::waitOrPop(Type & item)
{
   std::unique_lock<std::mutex> queueLock(this->lockQueue);
   this->data_cond_var.wait(queueLock,[this]{
       return !this->dataQueue.empty();
   });
   item = std::move(dataQueue.front());
   dataQueue.pop();
}

template<typename Type>
std::unique_ptr<Type> ThreadSafeQueue<Type>::waitOrPop()
{
    std::unique_lock<std::mutex> queueLock(this->lockQueue);
    this->data_cond_var.wait(queueLock,[this]{
        return !this->dataQueue.empty();
    });
    std::unique_ptr<Type> data_pop(std::make_shared<Type>
                   (std::move(this->dataQueue.front())));
    dataQueue.pop();
    return std::move(data_pop);
    
}

template<typename Type>
bool ThreadSafeQueue<Type>::tryPop(Type &item)
{
      if (lockQueue.try_lock()){
          std::lock_guard<std::mutex> queueLock(this->lockQueue,std::adopt_lock);
          if (dataQueue.empty())
              return false;
          try {
            item = std::move(dataQueue.front());
            dataQueue.pop();
          }
          catch(...){
              return false;
          }

          return true;
          }
      else {
          return false;
      }

}

template<typename Type>
std::unique_ptr<Type> ThreadSafeQueue<Type>::tryPop()
{
    if (lockQueue.try_lock()){
        std::lock_guard<std::mutex> queueLock(this->lockQueue,std::adopt_lock);
        if (dataQueue.empty())
            return std::shared_ptr<Type>();
        try {
             std::unique_ptr<Type> data_pop(std::make_shared<Type>
                           (std::move(this->dataQueue.front())));
            dataQueue.pop();
            return data_pop;
        }
        catch(...){
            return std::shared_ptr<Type>();
        }

        return true;
        }
    else {
        return false;
    }
}

template<typename Type>
bool ThreadSafeQueue<Type>::empty() const
{
   std::lock_guard<std::mutex> queueLock(this->lockQueue);
   return dataQueue.empty();
   
}

template<typename Type>
void ThreadSafeQueue<Type>::notify()
{
    //this->data_cond_var.notify_one();
}

template<typename Type>
ThreadSafeQueue<Type>::~ThreadSafeQueue()
{
    //notify();
}

template<typename Type>
void ThreadSafeQueue<Type>::clear()
{
    std::unique_lock<std::mutex> queueLock(this->lockQueue);



}


#endif // THREADSAFEQUEUE_H
