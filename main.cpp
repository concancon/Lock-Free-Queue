
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
    
    
    void getInput(std::future<void>& futureFromBackground){
        

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
            
            futureFromBackground.get();
            std::cout<< "we are back. one loop has been completed" << std::endl;
            
            inputPromise = std::promise<void>{};
            //we are now so far that we can delete the promise. But how do we assign the new corresponding future?
            
        }
    }
  
    std::deque<int> inputQueue; //should be private?
    std::promise<void> inputPromise;
    std::future<void> inputFuture;
    
};



class Background{
    
public:
    Background(){
  
    }
    
    void process(std::future<void>& futureFromInputOutput){
        
        std::cout<< "waiting for future.get  "  <<std::endl;
        futureFromInputOutput.get();
        std::cout<< "future was got: "  <<std::endl;
        outputPromise.set_value();
    
    }
    
    std::deque<int> outputQueue;
    std::promise<void> outputPromise;

};



int main() {
    
    
    InputOutput io;
    
    Background background;
    
    auto backgroundFuture = background.outputPromise.get_future();
    std::thread inputOutputThread(&InputOutput::getInput, &io, std::ref(backgroundFuture));

    auto inputOutputFuture = io.inputPromise.get_future();
    std::thread backgroundThread(&Background::process, &background, std::ref(inputOutputFuture));

    inputOutputThread.join();
    backgroundThread.join();

    
    
}
