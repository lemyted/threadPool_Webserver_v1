#include "../includes/RequestBacklog.hpp"

void RequestBacklog::pushRequest(const Request req) 
{
  waitWhileIsBeingAccessed();
  isBeingAccessed = true;
  requests.push_back(req);
  isBeingAccessed = false;
}

void RequestBacklog::popRequest(Request &req) 
{
  waitWhileIsBeingAccessed();
  isBeingAccessed = true;
  req = requests[0];
  requests.erase(requests.begin());
  isBeingAccessed = false;
}

int RequestBacklog::getSize() 
{
  return requests.size();
}

void RequestBacklog::waitWhileIsBeingAccessed() 
{
  while (isBeingAccessed.load());  
}
