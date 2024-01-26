#include <iostream>
#include "Services/network.hpp"
#include "Services/network.cpp"

int main()
{
  Network* net = new Network();
  net->SendGETRequest();
  return 0;
}