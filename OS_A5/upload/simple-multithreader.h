#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

typedef struct{
  int low;
  int high;
  std::function<void(int)> lambda;

}thread_args;

typedef struct{
  int low1;
  int high1;
  int low2;
  int high2;
  std::function<void(int,int)> lambda;
}thread_args_12;


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
    for (int j = t->low2; p < t->high2; j++){
      printf("huh");
      t->lambda(p,j);
    }
  }
  return NULL;
}

void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
  pthread_t tid[numThreads];
  thread_args args[numThreads];
  int chunk = (high - low) / numThreads;
  int rem = (high - low) % numThreads;
  for (int i=0; i<numThreads; i++){
    args[i].low = i*chunk;
    args[i].high = args[i].low + chunk;
    if (i == numThreads - 1){
      args[i].high += rem;
    }
    args[i].lambda = lambda;
    pthread_create(&tid[i], NULL, thread_func, (&args[i]));
  }
  for (int i=0; i<numThreads; i++){
    pthread_join(tid[i], NULL);
  }
}

void parallel_for(int low1, int high1,  int low2, int high2, std::function<void(int, int)>  &&lambda, int numThreads){
  pthread_t tid[numThreads];
  thread_args_12 args[numThreads];
  int size = high2 - low2;
  int chunk1 = (high1 - low1) / numThreads;
  // int chunk2 = size;
  int rem1 = (high1 - low1) % numThreads;
  // int rem2 = (high2 - low2) % numThreads;
  for (int i=0; i<numThreads; i++){
    args[i].low1 = i*chunk1;
    args[i].high1 = args[i].low1 + chunk1;
    args[i].low2 = low2;
    args[i].high2 = high2;
    if (i == numThreads - 1){
      args[i].high1 += rem1;
      // args[i].high2 += rem2;
    }
    args[i].lambda = lambda;
    pthread_create(&tid[i], NULL, thread_func_12, (&args[i]));
  }
  for (int i=0; i<numThreads; i++){
    pthread_join(tid[i], NULL);
  }  
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


