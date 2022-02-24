#include <chrono>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void getInputAndPrint(std::promise<int> number,
                      std::future<int> resultFromMathThread) {
    std::cout << "# getString() start\n";
    std::cout << "# getString(): Please enter an int. Whenever...\n";
    int input = 0;
    // std::getline(std::cin, input);

    std::cin >> input;

    number.set_value(input);

    std::cout << "received " << resultFromMathThread.get() << std::endl;
}

void modifyInput(std::promise<int> resultPromise, std::future<int> userInput) {

    std::cout << "now we are in the math thread" << std::endl;

    auto input = userInput.get();

    resultPromise.set_value(input);
}

int main() {

    std::promise<int> key_promise;
    std::future<int> key_future = key_promise.get_future();

    std::promise<int> math_promise;
    std::future<int> math_future = math_promise.get_future();

    std::thread work_thread(getInputAndPrint, std::move(key_promise),
                            std::move(math_future));

    std::thread math_thread(modifyInput, std::move(math_promise),
                            std::move(key_future));

    work_thread.join();
    math_thread.join();
}
