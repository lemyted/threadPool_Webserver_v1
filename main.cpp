#include <thread>
#include "includes/Server.hpp"

int main() 
{
  Server* server = new Server();
  std::string info = "Cores available: ";
  const auto processor_count = std::thread::hardware_concurrency();
  server->writeLog(info.append(std::to_string(processor_count)), "INFOS", __FILE__, __LINE__);
  server->start();
  server->~Server();    
}
