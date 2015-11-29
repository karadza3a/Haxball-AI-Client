//
//  main.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/25/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "GlobalState.hpp"
#include "Communicator.hpp"
#include <cmath>

int main(int argc, char *argv[]) {

  srand((unsigned int)time(0));
  string username = "Bot  ";
  username[3] = 'A' + rand() % 26;
  username[4] = '0' + rand() % 10;

  Communicator *comm = new Communicator("localhost", 16000, 15000, username);

  comm->sendLogin();

  GlobalState GS(comm);

  for (int i = 0; i < 1000000; i++) {
    GS.updateState();
    cout << GS.myPlayer.pos.x << " ";
    cout << GS.myPlayer.pos.y << " ";
    cout << GS.ballPos.x << " ";
    cout << GS.ballPos.y << " ";
    cout << endl;
    float dx = GS.myPlayer.pos.x - GS.ballPos.x;
    float dy = GS.myPlayer.pos.y - GS.ballPos.y;

    int key = 0;
    if (abs(dx) > 3.5) {
      key |= (dx > 0) ? KEY_LEFT : KEY_RIGHT;
    }
    if (abs(dy) > 3.5) {
      key |= (dy > 0) ? KEY_DOWN : KEY_UP;
    }
    if (abs(dx) < 4.5 && abs(dy) < 4.5)
      key |= KEY_SHOOT;

    comm->sendCommand(key);
  }

  return 0;
}
