#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <memory>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>

#include "../includes/Request.hpp"

#define BUFFSIZE 10000

class Server {
  protected:
  const char port[5] = "3000";
  int family = AF_UNSPEC; 
  int socktype = SOCK_STREAM;
  int flags = AI_PASSIVE;
  struct addrinfo hints;
  struct addrinfo *serverInfos = nullptr;
  std::string errorLogType = "ERROR";
  std::string infoLogType = "INFOS";
  int isPortReusable = 1;
  bool isRunning = false;  
  int backlog = 10;
  int serverFd = 0;

  std::string addBrackets(std::string str);
  void bindServer();
  void setsockoptServer();
  void listenServer();

  public:

  virtual void initLog();
  virtual void writeLog(std::string log, std::string type, std::string filename, int line);
  virtual void handleRequests();
  virtual int start();
  virtual int stop();

  Server() 
  {
    serverInfos = (addrinfo*)malloc(sizeof (addrinfo*));

    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    int status;

    if ((status = getaddrinfo(NULL, port, &hints, &serverInfos)) != 0) {
      writeLog(gai_strerror(status), errorLogType, __FILE__, __LINE__);
      exit(1);
    } else {
      writeLog("Server infos gathered successfully", infoLogType, __FILE__, __LINE__);      
    }

    serverFd = socket(serverInfos->ai_family, serverInfos->ai_socktype, serverInfos->ai_protocol);
  }

  ~Server() 
  {
    freeaddrinfo(serverInfos);
  }
};
