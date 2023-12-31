#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <chrono>

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}
//creating structs in order to pass the arguments as a single pointer
//struct for parallel_for with signature containing <void(int)> lambda
typedef struct{ 
  int low;
  int high;
  std::function<void(int)> lambda;

}thread_args;

//struct for parallel_for with signature containing <void(int,int)> lambda
typedef struct{
  int low1;
  int high1;
  int low2;
  int high2;
  std::function<void(int,int)> lambda;
}thread_args_12;

//functions which get called by the pthread_create, for both parallel_for signatures
void *thread_func(void* ptr){
  thread_args * t = ((thread_args *) ptr); 
  for (int p = t->low; p < t->high; p++){
    t->lambda(p);
  }
  return NULL;
}

void *thread_func_12(void* ptr){
  thread_args_12 * t = ((thread_args_12 *) ptr); 
  for (int p = t->low1; p < t->high1; p++){
    for (int j = t->low2; j < t->high2; j++){
      t->lambda(p,j);
    }
  }
  return NULL;
}

//parallel_for function for the vector type programs
//Overflow condition handled to ensure load on one thread doesn't occur
void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
  std::chrono::time_point<std::chrono::steady_clock> start;
  start = std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> end;
  if (high - low < 1024){
    for (int i = low; i < high; i++){
      lambda(i);
    }
  }
  else{
    int flag = 0;
    pthread_t tid[numThreads];
    thread_args args[numThreads];
    int chunk = (high - low) / numThreads;
    int rem = (high - low) % numThreads;
    if (numThreads > (high - low)){
      chunk = 1;
      rem = 0;
      flag = 1;
    }
    for (int i=0; i<numThreads; i++){
      args[i].low = i*chunk;
      args[i].high = args[i].low + chunk;
      if (i == numThreads - 1){
        args[i].high += rem;
      }
      args[i].lambda = lambda;
      if (flag == 1 && i >= (high - low)){
        args[i].low = 0;
        args[i].high = 0;
        args[i].lambda = lambda;
      }
      if (pthread_create(&tid[i], NULL, thread_func, (&args[i])) != 0){
        perror("Error: pthread_create");
      }
    }
    for (int i=0; i<numThreads; i++){
      if (pthread_join(tid[i], NULL) != 0){
        perror("Error: pthread_join");
      }
    }
  }
  end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "parallel_for execution time: ";
  std::cout << elapsed_seconds.count() << "s\n";
}

//parallel_for function for the matrix type programs
//Overflow condition handled to ensure load on one thread doesn't occur
void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)>  &&lambda, int numThreads){
  std::chrono::time_point<std::chrono::steady_clock> start;
  start = std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> end;
  if (high1 - low1 < 16 && high2 - low2 < 16){
    for (int i=low1; i<high1; i++){
      for (int j=low2; j<high2; j++){
        lambda(i,j);
      }
    }
  }
  else{
    int flag = 0;
    pthread_t tid[numThreads];
    thread_args_12 args[numThreads];
    int chunk1 = (high1 - low1) / numThreads;
    if (numThreads > high1 - low1){
      numThreads = high1 - low1;
      chunk1 = (high1 - low1) / numThreads;
    }
    int rem1 = (high1 - low1) % numThreads;
    if (numThreads > (high1 - low1)){
      chunk1 = 1;
      rem1 = 0;
      flag = 1;
    }
    for (int i=0; i<numThreads; i++){
      args[i].low1 = i*chunk1;
      args[i].high1 = args[i].low1 + chunk1;
      args[i].low2 = low2;
      args[i].high2 = high2;
      if (i == numThreads - 1){
        args[i].high1 += rem1;
      }
      args[i].lambda = lambda;
      if (flag == 1 && i >= (high1 - low1)){
        args[i].low1 = 0;
        args[i].high1 = 0;
        args[i].low2 = 0;
        args[i].high2 = 0;
        args[i].lambda = lambda;
      }
      if (pthread_create(&tid[i], NULL, thread_func_12, (&args[i])) != 0){
        perror("Error: pthread_create");
      }
    }
    for (int i=0; i<numThreads; i++){
      if (pthread_join(tid[i], NULL) != 0){
        perror("Error: pthread_join");
      }
    }
  }
  end = std::chrono::steady_clock::now();
  const std::chrono::duration<double> elapsed_seconds{end - start};
  std::cout << "parallel_for execution time: ";
  std::cout << elapsed_seconds.count() << "s\n";
}

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
  
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main


