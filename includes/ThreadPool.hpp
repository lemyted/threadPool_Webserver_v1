#include <thread>
#include <iostream>
#include <list>
#include <map>

enum ThreadState { RUNNING, READY, STOP };

class ThreadPool
{

private:

  int numThreads = 1;

public:

  ThreadPool(int cores) { numThreads = cores; }

  std::map<std::thread::id, ThreadState, std::thread> threadManager;

  int getNumThreads() { return ThreadPool::numThreads; }

  void makePool(int numThreads);
};

