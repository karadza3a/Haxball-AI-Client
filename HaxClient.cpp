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

    //    cout << GS.myPlayer.pos.x() << " ";
    //    cout << GS.myPlayer.pos.y() << " ";
    //    cout << GS.ballPos.x() << " ";
    //    cout << GS.ballPos.y() << " ";
    //    cout << endl;
    double dx = GS.myPlayer.pos.x() - GS.ballPos.x();
    double dy = GS.myPlayer.pos.y() - GS.ballPos.y();

    int key = 0;
    if (abs(dx) > 2.5) {
      key |= (dx > 0) ? KEY_LEFT : KEY_RIGHT;
    }
    if (abs(dy) > 2.5) {
      key |= (dy > 0) ? KEY_DOWN : KEY_UP;
    }

    Point post1(GS.oppGoal.x, GS.oppGoal.yMin + GS.constPostRadius);
    Point post2(GS.oppGoal.x, GS.oppGoal.yMax - GS.constPostRadius);

    Segment goalLine(post1, post2);
    Ray shootingRay(GS.myPlayer.pos, GS.ballPos);

    auto result = intersection(shootingRay, goalLine);

    if (result)
      key |= KEY_SHOOT;
    else {
      for (int i = -1; i < 2; i += 2) {
        Point p1(0, GS.constMaxY * i);
        Point p2(1, GS.constMaxY * i);
        Line sideLine(p1, p2);

        auto result = intersection(shootingRay, sideLine);
        if (result) {
          Point inters = boost::get<Point>(result.get());
          double dx = inters.x() - GS.myPlayer.pos.x();
          dx *= 1.2;
          Point p3(inters.x() + dx, GS.myPlayer.pos.y());
          Ray bounceRay(inters, p3);

          auto result = intersection(bounceRay, goalLine);

          if (result)
            key |= KEY_SHOOT;

          // cannot aim at both sidelines
          break;
        }
      }
    }

    comm->sendCommand(key);
  }

  return 0;
}
