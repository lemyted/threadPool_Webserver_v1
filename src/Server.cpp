#include "../includes/Server.hpp"

std::string Server::addBrackets(std::string str) 
{
  char lastChar = str[str.length() - 1];
  std::string trimmedStr = lastChar == '\n' ? str.replace(str.length() - 1, 1, "") : str;
  return "[" + trimmedStr + "]"; 
}

void Server::bindServer() 
{
  if (bind(serverFd, serverInfos->ai_addr, serverInfos->ai_addrlen) == -1) {
    writeLog("Unable to bind socket", ERROR_LOG, __FILE__, __LINE__);
    exit(1);
  } else {
    writeLog("Socket binded successfully", INFO_LOG, __FILE__, __LINE__);
  }
}

void Server::setsockoptServer() 
{
  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &isPortReusable, sizeof isPortReusable) == -1) {
    writeLog("Unable to setsockopt", ERROR_LOG, __FILE__, __LINE__);
    perror("setsockopt");
    exit(1);
  } else {
    writeLog("Successfully setsockopt", INFO_LOG, __FILE__, __LINE__);
  }
}

void Server::listenServer() 
{
  if (listen(serverFd, backlog) == -1) {
    writeLog("Unable to listen with socket", ERROR_LOG, __FILE__, __LINE__);
    exit(1);
  } else {
    writeLog("Successfully listening with socket", INFO_LOG, __FILE__, __LINE__);
  }
}

int Server::acceptRequest() 
{
  struct sockaddr_storage clientAddr;
  socklen_t addrSize = sizeof clientAddr;
  int clientFd = accept(serverFd, (struct sockaddr *) &clientAddr, &addrSize);

  if (clientFd == -1) {
    writeLog("Unable to accept request", ERROR_LOG, __FILE__, __LINE__);
    exit(1);
  } else {
    writeLog("Successfully listening with socket", ERROR_LOG, __FILE__, __LINE__);
  }

  return clientFd;
}

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

Request Server::getRequest() 
{
  int clientFd = acceptRequest();
  char buff[BUFFSIZE];
  int result = recv(clientFd, &buff, BUFFSIZE, 0);
  std::string rawReq = std::string(buff);

  Request req = Request(rawReq);
  req.parseRequest();
  req.setValue(CLIENT_ID, std::to_string(clientFd));

  writeLog(rawReq + "\n-----", INFO_LOG, __FILE__, __LINE__);  
  writeLog(req.toString() + "\n-----", INFO_LOG, __FILE__, __LINE__);

  return req;
}

void Server::addRequest(Request req)
{
  reqsToHandle->pushRequest(req);
  writeLog("Requests in backlog: " + std::to_string(reqsToHandle->getSize()), INFO_LOG, __FILE__, __LINE__);
}

void Server::handleRequests() 
{
  Request req = getRequest();
  addRequest(req);

  // TO REMOVE
  if (req.getValue(HTTP_TYPE) == "DELETE") 
  {
    writeLog("Exiting", INFO_LOG, __FILE__, __LINE__);
    stop();
  }
}

void Server::handleResponses() 
{
  Request req = {0};
  int clientFd = std::stoi(req.getValue(CLIENT_ID));
  char buff[] = "HELLO";

  reqsToHandle->popRequest(req);
  send(clientFd, &buff, sizeof (buff), MSG_CONFIRM);
  close(clientFd);
}

void Server::start() 
{
  bindServer();
  setsockoptServer();
  listenServer();
}

void Server::stop() 
{
  isRunning = false;
  close(serverFd);
}

void Server::run() 
{
  isRunning = true;
  std::string coresMsg = "Cores available: ";
  writeLog(coresMsg.append(std::to_string(cores)), INFO_LOG, __FILE__, __LINE__);
  std::string msgRunnig = "Server is running on port ";
  writeLog( msgRunnig.append(PORT), INFO_LOG, __FILE__, __LINE__);

  while (isRunning)
  {
    handleRequests();
    handleResponses();    
  }
}

void Server::serve() 
{
  start();
  run();
}
