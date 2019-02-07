#include <iostream>
#include "GenericTask.h"
#include <functional>
#include <future>
#include <ThreadSafeQueue.h>
#include <ThreadPool.h>
#include <sstream>
#include <cmath>
class hello{
    public:
        hello() = default;
        float num(int i){
            return
                    float(std::sin(i)*20.223)/ std::tan(i*10.90);
        }

};
float func(int x){
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    std::stringstream msg;
//    msg<< std::endl <<"Hello " << x << std::endl;
//    std::cout<< msg.str();
    return
            float(std::sin(x+1))/ std::tan(x*10.90);


    //return 6;
}

int main()
{
    {
       // ThreadPool pool(0);
    }


    ThreadPool pool(5);
    std::vector<std::future<float>> fut;
    int num = 9900000;
    for (int i = 0 ; i < num ; i++){
        fut.push_back(pool.enqueueTask(func,(i+1)*10));
    }
    std::cout << fut.size() << std::endl;
//    for (int i = 0 ;  fut.size() != 0; i++ ){
//        std::cout << i+1 << " " << (*fut.begin()).get() <<std::endl;
//        fut.erase(fut.begin());
//        std::cout << fut.size() << " is  left" << std::endl;

//    }

    //fut.clear();

    //std::cout << "DONE";
    //return 0;
    //exit(0);



}

