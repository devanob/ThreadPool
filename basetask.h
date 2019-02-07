#ifndef BASETASK_H
#define BASETASK_H
#include <memory>


//This will serve as A Base Task For Our Task
class BaseTask
{
    public:
        BaseTask()= default;
        virtual ~BaseTask() {}
        virtual void taskCall() = 0;


};





#endif // BASETASK_H
