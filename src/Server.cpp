#include "../includes/Server.hpp"

// PROTECTED
std::string Server::addBrackets(std::string str) 
{
  char lastChar = str[str.length() - 1];
  std::string trimmedStr = lastChar == '\n' ? str.replace(str.length() - 1, 1, "") : str;
  return "[" + trimmedStr + "]"; 
}

void Server::bindServer() 
{
  if (bind(serverFd, serverInfos->ai_addr, serverInfos->ai_addrlen) == -1) {
    writeLog("Unable to bind socket", errorLogType, __FILE__, __LINE__);
    exit(1);
  } else {
    writeLog("Socket binded successfully", infoLogType, __FILE__, __LINE__);
  }
}

void Server::setsockoptServer() 
{
  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &isPortReusable, sizeof isPortReusable) == -1) {
    writeLog("Unable to setsockopt", errorLogType, __FILE__, __LINE__);
    perror("setsockopt");
    exit(1);
  } else {
    writeLog("Successfully setsockopt", infoLogType, __FILE__, __LINE__);
  }
}

void Server::listenServer() 
{
   if (listen(serverFd, backlog) == -1) {
    writeLog("Unable to listen with socket", errorLogType, __FILE__, __LINE__);
    exit(1);
  } else {
    writeLog("Successfully listening with socket", infoLogType, __FILE__, __LINE__);
  }
}

// PUBLIC
void Server::initLog() 
{
  
}

void Server::writeLog(std::string log, std::string type, std::string filename, int line) 
{
  time_t now = time(0);

  std::cout 
    << addBrackets(ctime(&now)) << ' ' 
    << addBrackets(type) << ' ' 
    << addBrackets(filename) << ' '
    << addBrackets(std::to_string(line)) << '\n'
    << log << '\n';   
}

void Server::handleRequests() 
{
  struct sockaddr_storage clientAddr;
  socklen_t addrSize = sizeof clientAddr;
  int clientFd = accept(serverFd, (struct sockaddr *) &clientAddr, &addrSize);
  
  char buff[BUFFSIZE];
  int result = recv(clientFd, &buff, BUFFSIZE, 0);
  std::string rawReq = std::string(buff);

  writeLog(rawReq + "\n-----", infoLogType, __FILE__, __LINE__);
  Request *req = new Request(rawReq);
  req->parseHttpString();
  writeLog(req->toString() + "\n-----", infoLogType, __FILE__, __LINE__);

  if (req->getValue("HttpType") == "DELETE") 
  {
    writeLog("Exiting", infoLogType, __FILE__, __LINE__);
    stop();
  }

  req->~Request();
  free(req);
}

int Server::start() 
{
  bindServer();
  setsockoptServer();
  listenServer();

  isRunning = true;
  std::string msgRunnig = "Server is running on port ";
  writeLog( msgRunnig.append(port), infoLogType, __FILE__, __LINE__);

  while (isRunning)
  {
    handleRequests();    
  }

  return 0;  
}

int Server::stop() 
{
  isRunning = false;
  close(serverFd);
  return 0;
}
