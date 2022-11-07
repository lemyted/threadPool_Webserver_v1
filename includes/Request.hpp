#include <string>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>

#define CLIENT_ID "ClientId"
#define HTTP_TYPE "HttpType"
#define HTTP_URL "HttpUrl"
#define HTTP_VERSION "HttpVersion"
#define CONTENT "Content"

class Request
{
protected:
  std::string rawReq;
  std::map<std::string, std::string> parsedHttp;

  virtual void parseHead(std::vector<std::string> &lines);
  virtual void parseParams(std::vector<std::string> &lines);
  virtual void parseContent(std::vector<std::string> &lines);

public:
  virtual void parseRequest();
  virtual std::string toString();
  virtual std::string getValue(std::string key);
  virtual void setValue(std::string key, std::string value); 

  Request(std::string raw)
  {
    rawReq = raw;
  }

  ~Request()
  {

  }
};
