#include <vector>
#include "Request.hpp"

class RequestCollection
{
protected:
  std::vector<Request> requests;

public:
  RequestCollection(/* args */);
  ~RequestCollection();

  Request popRequest();
  Request pushRequest(Request req);
  Bool isEmpty();
};

RequestCollection::RequestCollection()
{
}

RequestCollection::~RequestCollection()
{
}
