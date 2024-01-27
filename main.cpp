#include <iostream>
#include "Services/network.hpp"
#include "Services/bot.hpp"

int main()
{
  Bot bot("329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0", ParseMode::HTML);
  Network net;
  net.ConnectBot(&bot);
  net.StartPolling();
  return 0;
}