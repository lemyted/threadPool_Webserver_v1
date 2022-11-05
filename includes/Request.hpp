#include <string>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>

class Request
{
protected:
  int startContent;
  std::string rawReq;
  std::map<std::string, std::string> parsedHttp;

  virtual void parseHead(std::vector<std::string> &lines);
  virtual void parseParams(std::vector<std::string> &lines);
  virtual void parseContent(std::vector<std::string> &lines);

public:
  virtual void parseHttpString();
  virtual std::string toString();
  virtual std::string getValue(std::string key); 

  Request(std::string raw)
  {
    rawReq = raw;
  }

  ~Request()
  {

  }
};
