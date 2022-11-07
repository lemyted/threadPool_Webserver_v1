#include <vector>
#include <atomic>

#include "Request.hpp"

class RequestBacklog
{
protected:
  std::vector<Request> requests = {};
  std::atomic<bool> isBeingAccessed = false;

public:
  RequestBacklog() 
  {

  }
  ~RequestBacklog() 
  {

  }

  virtual void popRequest(Request &req);
  virtual void pushRequest(const Request req);
  virtual int getSize();
  virtual void waitWhileIsBeingAccessed();
};
