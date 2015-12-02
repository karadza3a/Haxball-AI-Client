//
//  Communicator.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "Communicator.hpp"

Communicator::Communicator(string serverHostname, short serverBroadcastPort,
                           short serverListenPort, string username)
    : serverHostname(serverHostname), username(username),
      serverBroadcastPort(serverBroadcastPort),
      serverListenPort(serverListenPort) {}

void Communicator::sendlogin() {
  long messageLen = username.length() + 2;
  char message[messageLen];
  sprintf(message, "l;%s", username.c_str());
  sendRaw(message);
}

void Communicator::sendCommand(int keys) {
  long messageLen = username.length() + 10;
  char message[messageLen];
  sprintf(message, "p;%s;%d", username.c_str(), keys);
  sendRaw(message);
}

void Communicator::sendRaw(const char *message) {

  long messageLen = strlen(message);

  try {
    PracticalSocket::UDPSocket sock;
    sock.sendTo(message, (int)messageLen, serverHostname, serverListenPort);
  } catch (PracticalSocket::SocketException &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}

char *Communicator::receiveRaw() {

  try {
    PracticalSocket::UDPSocket sock(serverBroadcastPort);

    string sourceAddress;
    unsigned short sourcePort;

    int bytesRcvd =
        sock.recvFrom(receivedMessage, BUFFSIZE, sourceAddress, sourcePort);

    receivedMessage[bytesRcvd] = '\0';

    //    cout << "Received from " << sourceAddress << ":" << sourcePort;

  } catch (PracticalSocket::SocketException &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return receivedMessage;
}
