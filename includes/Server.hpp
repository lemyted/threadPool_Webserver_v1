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
#include <thread>

#include <boost/algorithm/string.hpp>

#include "../includes/RequestBacklog.hpp"

#define BUFFSIZE 10000
#define ERROR_LOG "ERROR"
#define INFO_LOG "INFOS"
#define PORT "3000"
#define FAMILY AF_UNSPEC
#define SOCK_TYPE SOCK_STREAM
#define SOCK_FLAGS AI_PASSIVE

class Server {
  protected:
  struct addrinfo hints = {0};
  struct addrinfo *serverInfos = {0};
  RequestBacklog *reqsToHandle = {0};
  int isPortReusable = 1;
  bool isRunning = false;  
  int backlog = 10;
  int serverFd = 0;
  int cores = 0;

  std::string addBrackets(std::string str);
  virtual void bindServer();
  virtual void setsockoptServer();
  virtual void listenServer();
  virtual int acceptRequest();
  virtual void initLog();
  virtual void writeLog(std::string log, std::string type, std::string filename, int line);
  virtual Request getRequest();
  virtual void addRequest(Request req);
  virtual void handleRequests();
  virtual void handleResponses();
  virtual void start();
  virtual void stop();
  virtual void run();

  public:
  virtual void serve();

  Server() 
  {
    cores = (int) std::thread::hardware_concurrency();
    serverInfos = (addrinfo*)malloc(sizeof (addrinfo*));

    memset(&hints, 0, sizeof hints);
    hints.ai_family = FAMILY;
    hints.ai_socktype = SOCK_TYPE;
    hints.ai_flags = SOCK_FLAGS;

    int status;

    if ((status = getaddrinfo(NULL, PORT, &hints, &serverInfos)) != 0) {
      writeLog(gai_strerror(status), ERROR_LOG, __FILE__, __LINE__);
      exit(1);
    } else {
      writeLog("Server infos gathered successfully", INFO_LOG, __FILE__, __LINE__);      
    }

    serverFd = socket(serverInfos->ai_family, serverInfos->ai_socktype, serverInfos->ai_protocol);
    reqsToHandle = (RequestBacklog*) malloc(sizeof (RequestBacklog));
    reqsToHandle = new RequestBacklog();
  }

  ~Server() 
  {
    freeaddrinfo(serverInfos);
    free(reqsToHandle);
  }
};
