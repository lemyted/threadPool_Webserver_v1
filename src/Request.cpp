#include "../includes/Request.hpp"

// to remove
#include "iostream"

void Request::parseHead(std::vector<std::string> &lines)
{
  // line 0 is not like the other lines, 
  // it has more than one key-value pair to add to the map
  std::vector<std::string> head;
  
  boost::split(head, lines[0], boost::is_any_of(" "));

  for (int i = 0; i < head.size(); i++) 
  {
    boost::trim(head[i]);
  }

  parsedHttp.insert(std::make_pair("HttpType", head[0]));
  parsedHttp.insert(std::make_pair("HttpUrl", head[1]));
  parsedHttp.insert(std::make_pair("HttpVersion", head[2]));
}

void Request::parseParams(std::vector<std::string> &lines) 
{
  for (int i = 0, pos = 0; i < lines.size(); i++) 
  {
    std::vector<std::string> keyValue;
    boost::split(keyValue, lines[i], boost::is_any_of(":"));
    boost::trim(keyValue[0]);
    boost::trim(keyValue[1]);
    std::cout << keyValue[0] << " : " << keyValue[1] << std::endl;

    parsedHttp.insert(std::make_pair(keyValue[0], keyValue[1]));
  }
}

void Request::parseContent(std::vector<std::string> &lines) 
{
  std::string content;

  for (int i = startContent; i < lines.size(); i++) 
  {
    std::string l = lines[i];
    boost::trim(l);
    content.append(l);
  }

  parsedHttp.insert(std::make_pair("Content", content));
}

void Request::parseHttpString() 
{
  std::vector<std::string> lines;
  std::map<std::string, std::string> parsedHttp;
  boost::split(lines, rawReq, boost::is_any_of("\n"));
  parseHead(lines);
  // parseParams(lines);
  // parseContent(lines);
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