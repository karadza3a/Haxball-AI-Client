//
//  main.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/25/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "GlobalState.hpp"
#include "Communicator.hpp"

int main(int argc, char *argv[]) {

  string username = "Bot";

  Communicator *comm = new Communicator("localhost", 16000, 15000, username);

  comm->sendLogin();

  GlobalState GS(comm);

  for (int i = 0; i < 1000000; i++) {
    GS.updateState();

    comm->sendCommand(8);
  }

  return 0;
}
