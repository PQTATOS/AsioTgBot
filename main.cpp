#include "AsioTgBot/network/include/tgbot.hpp"
#include "AsioTgBot/network/include/dispatcher.hpp"

int main()
{
  tgbot bot("6329343331:AAER5UjGOmLh_G7wBbEG7VNtBvyYQBD84Z0", parse_mode::HTML);
  dispatcher dispatch;
  dispatch.add_bot(&bot);

  dispatch.start();
  return 0;
}
