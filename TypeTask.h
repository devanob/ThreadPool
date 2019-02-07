#ifndef TYPETASK_H
#define TYPETASK_H
#include "basetask.h"
template<typename FunctionType>
class TypeTask : public BaseTask
{
    public:
        TypeTask() = delete;
        void taskCall();
        FunctionType functional;
        TypeTask(FunctionType&& toMoveObject) :
            functional(std::move(toMoveObject)){

        }

};

template <class FunctionType>
void TypeTask<FunctionType>::taskCall(){
    functional();
}

#endif // TYPETASK_H
