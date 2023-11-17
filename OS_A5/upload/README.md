# OS Assignment 5 – SimpleMultiThreader
Group 60 | Tanish Verma – 2022532 | Kirti Jain -2022250
This assignment aims to create a simple multithreader in cpp that  uses lambda functions as parameters, passes them to other functions, and executes them in parallel using pthreads. 
It essentially showcases a basic parallel execution mechanism using where threads are created accoding to the size of the input and rach thread carries out the lambda function parallel.


# Structures used:
# thread_args struct
This Struct contains 3 members: low and high are integer types representing the lower and upper boundary indexes. 
std::function<void(int)> lambda takes an integer argument and returns a void type. It takes a lambda function that operates on integers. 
Similarly, 
# thread_args_12
contains members similar to the previous one except it has 2 pairs of ranges, low, high, low1 and high1. 

Functions used:
# thread_func 
Contains the routine that is meant to be executed by individual threads.
This function expects a void pointer as a parameter. The pointer is cast as ‘thread_args’ and assigned to t. This helps us to access fields like low, high, time, the lambda function, etc.
The loop iterates through the low to high integer range and the function lambda is called using pointer t and the function is applied on every iteration of the loop, inside a thread.

# thread_func_12
Contains the function meant to be executed by individual threads just like the previous one.
Similar to the last function, this one expects a void pointer as a parameter and it is casted too. 
There is a dual loop, and the lambda function in this function is called and works on the pairs of integers iterated by the inner and outer loop.

These two functions are executed by every thread.
# void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads)
The start time is saved in a variable when the parallel_for function is starts execution.
The function takes low, high ranges, a lambda function and the number of threads as a parameter.
This function has a parallelizing loop to be run in ‘numThreads’ number of threads at the same time during execution.
An array of threads is declared, named ‘tid’. This array will hold the thread identifiers for multiple threads that will be created in this parallel_for fuction.  
The function first checks the range. If the range  is smaller, there is no need for parallelizing and the lambda function is executed sequentially for each integer i.
If the range is larger(>1024), threads are created and the number of chunks are calculated using the difference of range divided by the number of threads to divide the workload between the threads.

The number of remainder threads are also calculated. 
The function a special condition (flag) if the number of threads is greater than the range itself. In this case, it handles the computation with overflow condition.
After this, the threads are created using the pthread struct, and the thread_functions, in which the lambda function is executed for each thread in the required iterations. The created thread is stored in the tid array, declared to store the identifiers for the threads.
# Overflow condition check:
If more number of threads are created than the required number/size, the variable for int low and int high are set as 0,0 in case the thread number is more than the size. 
That is, if 1026 are numThreads and 1024 is size, then 1026 threads are created, but since work cannot be evenly divided, 1024 threads perform the computations and remaining 2 threads are called with high and low values as 0,0.


It is made sure that the main thread doesn’t proceed further until all the previous threads have completed their task using pthread_join. If so happens, and error is thrown. Using ptherad_join, all the threads are synchronised.
Error checks for successful thread creation as well as excess thread creation are made.
At the end, the end time is calculated.  Using the start time and end time, execution time is calculated.


# void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)> &&lambda, int numThreads)
This function takes 2 pairs of range boundaries, a lambda function to execute on every thread, and the number of threads created as parameters.
The start time is saved in a variable when the parallel_for function starts execution.
Similar to the previous function, the range size is checked. If the range size of both the boundaries <16, the lambda function is executed sequentially without parallelizing. If the range is larger, then a pthread array is declared to store the identifiers of threads that will be created.
Chunk1 and rem1 are calculated to distribute the workload depending on the number of threads created.
This function also handles a special condition (flag) if the number of threads is greater than the range itself.
Similar to the previous function described, both the range boundaries are set to 0. This way, overflow is handled.
Using an inner and outer for loop, the 2D matrix is divided to segments and each segment is assigned to a thread created using ‘thread_args_12’ struct. 
only the outer loop is parallelized, that is, in lambda(p,j), p is parallelized, j always goes from low2 to high2 inside the thread_func_12
Error checks are done while creating and joining threads and errors are thrown.
end time is calculated to calculate the runtime.

Total time taken by each parallel_for is printed whenever a parallel_for gets executed completely.

#Contributions-
Tanish Verma (2022532)- thread_func_12, parallel_for(dual ranges), parallel_for(single range), documentation
Kirti Jain(2022250)-thread_func, parallel_for(single range), documentation
Repo link- https://github.com/VerTanish/OS_A5/
