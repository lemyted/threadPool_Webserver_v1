#include <thread>
#include "../includes/ThreadState.hpp"

class WrappedThread
{
protected:
  ThreadState threadStatus;

public:
  WrappedThread(/* args */);
  ~WrappedThread();

  virtual ThreadState getStatus();
  virtual void execute();
  virtual void start();
  virtual void stop();
};

WrappedThread::WrappedThread()
{
}

WrappedThread::~WrappedThread()
{
}
