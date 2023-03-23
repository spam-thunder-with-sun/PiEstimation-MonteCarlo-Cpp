//STD
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>
#include <random>
using namespace std;
using namespace std::chrono;

void calcPI(long totNum, std::atomic<long>* inside)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    double num1, num2;
    long _inside = 10;
    for (long i = 0; i < totNum; i++)
    {
        num1 = dis(gen);
        num2 = dis(gen);

        if (num1 * num1 + num2 * num2 <= 1)
            _inside++;
    }

    *inside += _inside;

    return;
}

long toCompute = (long)2 << 35;
std::atomic<long> inside = {0}; 

int main(int argc, char **argv) 
{
    const int processor_count = std::thread::hardware_concurrency();
    vector<std::thread> task;
    if(processor_count > 1)
        task.resize(processor_count - 1);
    else
        task.resize(1);
    long numXtask = toCompute / task.size();
    toCompute = numXtask * task.size();

    duration<double> elapsed_seconds;

    cout << "Compute Pi with MonteCarlo 2.0" << endl;
    cout << "Number of logical core: " << std::to_string(processor_count) << endl;
    cout << "To compute: " << toCompute << endl;
    
    auto start = steady_clock::now();

    for (int i = 0; i < task.size(); i++)
        task[i] = std::thread(calcPI, numXtask, &inside);

    for (std::thread& foo : task)
        foo.join();

    auto end = steady_clock::now();
    elapsed_seconds = end-start;
    cout << "Time: "<< elapsed_seconds.count() << " ";
    cout << "Interactions: "<< toCompute << " ";
    cout << "Inside: "<< inside << " ";
    cout.precision(10);
    cout << "Pi: "<< fixed << (long double)inside / (toCompute >> 2) << endl;

    return 0;
}


