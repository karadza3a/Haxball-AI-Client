//
//  main.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/25/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "Communicator.hpp"

int main(int argc, char *argv[]) {

  Communicator *comm = new Communicator("localhost", 16000, 15000);
  comm->sendLogin("Bot");

  sleep(2);

  comm->sendCommand("Bot", 2);

  sleep(2);

  cout << comm->receiveRaw() << endl;

  return 0;
}
