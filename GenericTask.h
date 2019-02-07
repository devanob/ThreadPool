#ifndef GENERICTASK_H
#define GENERICTASK_H
#include <TypeTask.h>

class GenericTask
{
    public:
        GenericTask() = default;
    template<typename FunctionType>
    GenericTask(FunctionType&& toMoveObject);
    GenericTask(GenericTask&& toMoveObject);
    GenericTask& operator=(GenericTask&& toMoveObject);
    void operator()();
    //Deleted Function
     GenericTask(const   GenericTask&)=delete;
     GenericTask(GenericTask&)=delete;
     GenericTask& operator=(const GenericTask&)=delete;
    private:
        std::unique_ptr<BaseTask> taskPtr;

};

//
void GenericTask::operator()(){
    this->taskPtr->taskCall();
}

template<typename FunctionType>
GenericTask::GenericTask(FunctionType&& toMoveObject) {
    this->taskPtr  = std::unique_ptr<BaseTask>(new TypeTask<FunctionType>(std::move(toMoveObject)));
}
GenericTask::GenericTask(GenericTask&& toMoveObject){
    this->taskPtr =  std::move(toMoveObject.taskPtr);
}

GenericTask &GenericTask::operator=(GenericTask &&toMoveObject)
{

    this->taskPtr = std::move(toMoveObject.taskPtr);
    return *this;

}
#endif // GENERICTASK_H
