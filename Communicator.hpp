//
//  Communicator.hpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#ifndef Communicator_hpp
#define Communicator_hpp

#include "PracticalSocket.h"
#include <iostream>
#include <cstdlib>
#include <set>

#define BUFFSIZE 10000
#define HOME 0
#define AWAY 1

class GlobalState;

class Communicator {
  string serverHostname;
  string username;
  short serverBroadcastPort, serverListenPort;
  char receivedMessage[BUFFSIZE];

  void sendRaw(const char *message);
  char *receiveRaw();

  friend GlobalState;

public:
  void sendLogin();
  void sendCommand(int keys);
  Communicator(string serverHostname, short serverBroadcastPort,
               short serverListenPort, string username);
};

#endif /* Communicator_hpp */
