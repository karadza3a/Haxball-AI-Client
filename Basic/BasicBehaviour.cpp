//
//  BasicBehaviour.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "BasicBehaviour.hpp"

BasicBehaviour::BasicBehaviour(GlobalState *GS) : GS(GS) {}

int BasicBehaviour::move() {
  double dx = GS->myPlayer.pos.x() - GS->ballPos.x();
  double dy = GS->myPlayer.pos.y() - GS->ballPos.y();

  int key = 0;
  if (std::abs(dx) > GS->constBallRadius + GS->constBallRadius) {
    key |= (dx > 0) ? KEY_LEFT : KEY_RIGHT;
  }
  if (std::abs(dy) > GS->constBallRadius + GS->constBallRadius) {
    key |= (dy > 0) ? KEY_DOWN : KEY_UP;
  }
  return key;
}
int BasicBehaviour::shoot() {
  Circle player(GS->myPlayer.pos,
                GS->constPlayerRadius * GS->constPlayerRadius);
  Circle kicker(GS->myPlayer.pos,
                GS->constKickerRadius * GS->constKickerRadius);
  Circle ball(GS->ballPos, GS->constBallRadius * GS->constBallRadius);

  if (!do_intersect(player, ball)) {
    return 0;
  }

  int key = 0;
  Point post1(GS->oppGoal.x, GS->oppGoal.yMin + GS->constBallRadius);
  Point post2(GS->oppGoal.x, GS->oppGoal.yMax - GS->constBallRadius);

  Segment goalLine(post1, post2);
  Ray shootingRay(GS->myPlayer.pos, GS->ballPos);

  auto result = intersection(shootingRay, goalLine);

  if (result)
    key |= KEY_SHOOT;
  else {
    for (int i = -1; i < 2; i += 2) {
      Point p1(0, GS->constMaxY * i);
      Point p2(1, GS->constMaxY * i);
      Line sideLine(p1, p2);

      auto result = intersection(shootingRay, sideLine);
      if (result) {
        Point inters = boost::get<Point>(result.get());

        // correcting due to ball velocity
        Point realIntersect(inters.x() + GS->ballVel.x() * .5, inters.y());

        double dx = realIntersect.x() - GS->myPlayer.pos.x();
        Point p3(realIntersect.x() + dx, GS->myPlayer.pos.y());
        Ray bounceRay(realIntersect, p3);

        auto result = intersection(bounceRay, goalLine);

        if (result)
          key |= KEY_SHOOT;

        // cannot aim at both sidelines
        break;
      }
    }
  }
  return key;
}
void BasicBehaviour::start() {
  for (int i = 0; i < 1000000; i++) {
    GS->updateState();
    int key = 0;
    key |= move();
    key |= shoot();
    GS->comm->sendCommand(key);

    if (key & KEY_SHOOT) {
      std::cout << time(0) << " poof" << std::endl;
    }
  }
}