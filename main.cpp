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

    cout << GS.myPlayer.pos.x() << " ";
    cout << GS.myPlayer.pos.y() << " ";
    cout << GS.ballPos.x() << " ";
    cout << GS.ballPos.y() << " ";
    cout << endl;
    double dx = GS.myPlayer.pos.x() - GS.ballPos.x();
    double dy = GS.myPlayer.pos.y() - GS.ballPos.y();

    int key = 0;
    if (abs(dx) > 2.5) {
      key |= (dx > 0) ? KEY_LEFT : KEY_RIGHT;
    }
    if (abs(dy) > 2.5) {
      key |= (dy > 0) ? KEY_DOWN : KEY_UP;
    }

    Point post1(GS.oppGoal.x, -GS.oppGoal.goalWidth / 2);
    Point post2(GS.oppGoal.x, GS.oppGoal.goalWidth / 2);

    Segment goalLine(post1, post2);
    Ray shootingLine(GS.myPlayer.pos, GS.ballPos);

    auto result = intersection(shootingLine, goalLine);

    if (result)
      key |= KEY_SHOOT;

    cout << endl;

    comm->sendCommand(key);
  }

  return 0;
}
