
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <deque>



class InputOutput{
    
public:
    
    InputOutput(){
        
        std::cout<< "our q is initialized and has a size of: " << inputQueue.size() << std::endl;
        
        
    }
    
    
    void getInput(){
        
        
        
        
        int input = 1;
        
        //inputPromise = std::promise<void>{};
        while(input){
            
            
            
            std::cout<< "please input a number " <<std::endl;
        
            std::cin >> input;
            
            if(input){
                
                
                inputQueue.push_front(input);
                inputPromise.set_value();
               
            }
            //dont push 0
            
            std::cout<< "now our q has size " << inputQueue.size() << std::endl;
        }
    }
  
    std::deque<int> inputQueue; //should be private?
    std::promise<void> inputPromise;
    
};



class Background{
    
public:
    Background(){
  
    }
    
    void process(std::future<void> future){
        
        std::cout<< "waiting for future.get  "  <<std::endl;
        future.get();
        std::cout<< "future was got: "  <<std::endl;
        
    }
    
    std::deque<int> outputQueue;
    std::future<void> futureFromInput;

};



int main() {
    
    
    InputOutput io;
    
    std::thread inputOutputThread(&InputOutput::getInput, &io);

    //auto future = io.inputPromise.get_future();
    std::thread backgroundThread(&Background::process, Background(), io.inputPromise.get_future());

    inputOutputThread.join();
    backgroundThread.join();

    
    
}
