// main file for testing

#include <iostream>
#include <functional>
#include <future>
#include <chrono>
#include <thread>
#include "que.h"

using namespace std::chrono_literals;

// local functions

/*
A simple task 1
*/
void simple_task_1();

/*
A simple task 2
*/
void simple_task_2();

/*
A simple task 3
*/
void simple_task_3();

/*
exec cycler function w/ std::function
*/
void execute(jinx::Safe_Thread<std::function<void()>> & runner, const int & exec_cycles);

int main(int argc, char ** argv)
{

    // Local variables

    const int exec_cycles = 20;
    jinx::Safe_Thread<std::function<void()>> exec1;

    // Setup

    std::cout << "Testing the parallel execution quenue.\n\n";

    std::cout << "Cores available: " << std::thread::hardware_concurrency() << "\n\n";

    // Body Exec

    execute(exec1, exec_cycles);        

    // Exit

    std::cout << std::endl;
    std::cout << "PRESS 'ENTER' TO CONTINUE...\n";
    std::cin.get();
    return EXIT_SUCCESS;

} // !main()

/*
A simple task 1
*/
void simple_task_1()
{
    std::cout << "EXEC T1: " << __FUNCTION__ << "| THREAD: " << std::this_thread::get_id() << '\n';
} // !simple_task_1()

/*
A simple task 2
*/
void simple_task_2()
{    
    std::cout << "EXEC T2: " << __FUNCTION__ << "| THREAD: " << std::this_thread::get_id() << '\n';
} // !simple_task_2()

/*
A simple task 3
*/
void simple_task_3()
{
    std::cout << "EXEC T3: " << __FUNCTION__ << "| THREAD: " << std::this_thread::get_id() << '\n';
} // !simple_task_3()

/*
exec cycler function w/ std::function
*/
void execute(jinx::Safe_Thread<std::function<void()>> & runner, const int & exec_cycles)
{
    for (int i = 0; i < exec_cycles; ++i)
    {
        if (i % 3 == 0)
        {
            runner.submit(simple_task_1);
            runner.submit(simple_task_2);
            runner.submit(simple_task_3);

            std::this_thread::sleep_for(100ms);
        }
        else if (i % 2 == 0)
        {
            runner.submit(simple_task_1);
            runner.submit(simple_task_2);

            std::this_thread::sleep_for(50ms);
        }
        else
        {
            runner.submit(simple_task_1);
            std::this_thread::sleep_for(10ms);
        }
        
    } // !exec cycle
} // !execute