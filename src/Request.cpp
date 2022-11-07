#include "../includes/Request.hpp"

void Request::parseHead(std::vector<std::string> &lines)
{
  std::vector<std::string> head;
  std::vector<std::string> keys {HTTP_TYPE, HTTP_URL, HTTP_VERSION};  
  boost::split(head, lines[0], boost::is_any_of(" "));

  for (int i = 0; i < head.size(); i++) 
  {
    boost::trim(head[i]);
    std::string key = i < head.size() ? keys[i] : "HeadElement" + std::to_string(i);
    parsedHttp.insert(std::make_pair(key, head[i]));
  }

  lines.erase(lines.begin());
}

void Request::parseParams(std::vector<std::string> &lines) 
{
  int i = 0;

  for (i = 0; i < lines.size(); i++) 
  {
    if (lines[i].length() <= 1) break;

    std::vector<std::string> keyValue;
    boost::split(keyValue, lines[i], boost::is_any_of(":"));
    boost::trim(keyValue[0]);
    std::string value = "";

    for (int j = 1; j < keyValue.size(); j++) value.append(keyValue[j]);

    boost::trim(value);
    parsedHttp.insert(std::make_pair(keyValue[0], value));
  }

  lines.erase(lines.begin(), lines.begin() + i + 1);
}

void Request::parseContent(std::vector<std::string> &lines) 
{
  std::string content;

  for (int i = 0; i < lines.size(); i++) 
  {
    boost::trim(lines[i]);
    content.append(lines[i]);
  }

  parsedHttp.insert(std::make_pair(CONTENT, content));
}

void Request::parseRequest() 
{
  std::vector<std::string> lines;
  std::map<std::string, std::string> parsedHttp;
  boost::split(lines, rawReq, boost::is_any_of("\n"));
  parseHead(lines);
  parseParams(lines);
  parseContent(lines);
}

std::string Request::toString() 
{
  std::string res = "";

  for (auto kv : parsedHttp) 
  {
    res.append(kv.first + " : " + kv.second + "\n");
  }

  return res;
}

std::string Request::getValue(std::string key) 
{
  auto pos = parsedHttp.find(key);

  if (pos == parsedHttp.end()) 
  {
    return "";
  }
  else 
  {
    return pos->second;
  }
}

void Request::setValue(std::string key, std::string value) 
{
  parsedHttp[key] = value;
}