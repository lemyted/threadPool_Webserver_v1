#include "includes/Server.hpp"

int main() 
{
  Server* server = new Server();
  server->serve();
  server->~Server();    
}
