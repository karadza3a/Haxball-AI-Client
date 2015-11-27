//
//  Communicator.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "Communicator.hpp"

Communicator::Communicator(string serverHostname, short serverBroadcastPort,
                           short serverListenPort)
    : serverHostname(serverHostname), serverBroadcastPort(serverBroadcastPort),
      serverListenPort(serverListenPort) {}

void Communicator::sendLogin(const char *username) {
  long messageLen = strlen(username) + 2;
  char message[messageLen];
  sprintf(message, "l;%s", username);
  sendRaw(message);
}

void Communicator::sendCommand(const char *username, int keys) {
  long messageLen = strlen(username) + 10;
  char message[messageLen];
  sprintf(message, "p;%s;%d", username, keys);
  sendRaw(message);
}

void Communicator::sendRaw(const char *message) {

  long messageLen = strlen(message);

  try {
    UDPSocket sock;
    sock.sendTo(message, (int)messageLen, serverHostname, serverListenPort);
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
}

char *Communicator::receiveRaw() {

  try {
    UDPSocket sock(serverBroadcastPort);

    string sourceAddress;
    unsigned short sourcePort;

    int bytesRcvd =
        sock.recvFrom(receivedMessage, BUFFSIZE, sourceAddress, sourcePort);

    receivedMessage[bytesRcvd] = '\0';

    //    cout << "Received from " << sourceAddress << ":" << sourcePort;

  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  return receivedMessage;
}