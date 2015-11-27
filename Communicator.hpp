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

#define BUFFSIZE 10000

class Communicator {
  string serverHostname;
  short serverBroadcastPort, serverListenPort;
  char receivedMessage[BUFFSIZE];
  void sendRaw(const char *message);

public:
  char *receiveRaw();
  void sendLogin(const char *username);
  void sendCommand(const char *username, int keys);
  Communicator(string serverHostname, short serverBroadcastPort,
               short serverListenPort);
};

#endif /* Communicator_hpp */
