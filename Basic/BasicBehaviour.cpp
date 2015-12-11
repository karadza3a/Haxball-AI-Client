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

  Ray shootingRay(GS->ballPos, GS->ballPos + (GS->ballPos - GS->myPlayer.pos));

  auto result = intersection(shootingRay, goalLine);

  if (result)
    key |= KEY_SHOOT;
  else {
    if (signof(GS->ballPos.x() - GS->myPlayer.pos.x()) ==
        signof(GS->oppGoal.x)) {

      Point intersX = predictSidelineShotIntersect();
      Point bounceY = predictSidelineBounceIntersect(intersX.x());
      if (std::abs(bounceY.y()) < GS->oppGoal.yMax - GS->constBallRadius)
        key |= KEY_SHOOT;
    }
  }
  return key;
}

Point BasicBehaviour::predictSidelineBounceIntersect(double interX) {
  Point p = GS->myPlayer.pos, b = GS->ballPos;
  Vector pv = GS->myPlayer.vel, bv = GS->ballVel, shv(p, b);

  Point sidep1(3, GS->constMaxY * signof(shv.y()));
  Point sidep2(4, GS->constMaxY * signof(shv.y()));
  Line outline(sidep1, sidep2);

  Ray idealRay(b, *new Point(b.x() + shv.x(), b.y() + shv.y()));
  auto result = intersection(idealRay, outline);
  Point idealInters = boost::get<Point>(result.get());

  double params[] = {
      1, idealInters.x(), 20 * signof(shv.y()), b.y(), b.x(), b.x() * b.x(),
      b.y() * b.y(), b.y() / b.x(), atan2(b.y(), b.x()), shv.x(), shv.y(),
      shv.x() * shv.x(), shv.y() * shv.y(),
      sqrt(shv.x() * shv.x() + shv.y() * shv.y()), shv.y() / shv.x(),
      atan2(shv.y(), shv.x()), pv.x(), pv.y(), pv.x() * pv.x(), pv.y() * pv.y(),
      sqrt(pv.x() * pv.x() + pv.y() * pv.y()), pv.y() / pv.x(),
      atan2(pv.y(), pv.x()), bv.x(), bv.y(), bv.x() * bv.x(), bv.y() * bv.y(),
      sqrt(bv.x() * bv.x() + bv.y() * bv.y()), bv.y() / bv.x(),
      atan2(bv.y(), bv.x()), interX};
  double theta[] = {
      0.5033,  2.4516,    4.0245,   2.6602,  1.4758,  0.02341, 0.31682, -17.595,
      5.3749,  -0.24473,  -2.3588,  -1.8794, 0.25524, 0.77455, -6.7255, -11.028,
      1.5136,  4.0408,    0.54652,  0.77314, -18.704, 0.06929, -11.394, 1.3659,
      -2.9084, -0.026569, -0.18733, 7.9073,  12.284,  -14.106, -3.6843};
  int n = sizeof(params) / sizeof(params[0]);
  double y = 0;
  for (int i = 0; i < n; i++) {
    y += params[i] * theta[i];
  }
  Point inters(0, y);
  return inters;
}
Point BasicBehaviour::predictSidelineShotIntersect() {
  Point p = GS->myPlayer.pos, b = GS->ballPos;
  Vector pv = GS->myPlayer.vel, bv = GS->ballVel, shv(p, b);

  Point sidep1(3, GS->constMaxY * signof(shv.y()));
  Point sidep2(4, GS->constMaxY * signof(shv.y()));
  Line outline(sidep1, sidep2);

  Ray idealRay(b, *new Point(b.x() + shv.x(), b.y() + shv.y()));
  auto result = intersection(idealRay, outline);
  Point idealInters = boost::get<Point>(result.get());

  double params[] = {
      1, idealInters.x(), 20 * signof(shv.y()), b.y(), b.x(), b.x() * b.x(),
      b.y() * b.y(), b.y() / b.x(), atan2(b.y(), b.x()), shv.x(), shv.y(),
      shv.x() * shv.x(), shv.y() * shv.y(),
      sqrt(shv.x() * shv.x() + shv.y() * shv.y()), shv.y() / shv.x(),
      atan2(shv.y(), shv.x()), pv.x(), pv.y(), pv.x() * pv.x(), pv.y() * pv.y(),
      sqrt(pv.x() * pv.x() + pv.y() * pv.y()), pv.y() / pv.x(),
      atan2(pv.y(), pv.x()), bv.x(), bv.y(), bv.x() * bv.x(), bv.y() * bv.y(),
      sqrt(bv.x() * bv.x() + bv.y() * bv.y()), bv.y() / bv.x(),
      atan2(bv.y(), bv.x())};
  double theta[] = {0.7969,  0.5076,  -1.8329,  -1.2616, 0.6156,   0.0154,
                    -0.0909, 0.5626,  1.3680,   -6.6277, -3.3512,  3.9349,
                    -1.1596, 1.8932,  9.5040,   -4.3404, 0.7068,   0.7535,
                    0.7857,  0.7220,  -19.1102, 0.0607,  0.3140,   0.3031,
                    1.1920,  -0.2138, -0.0846,  7.4943,  -12.0288, 17.5540};
  int n = sizeof(params) / sizeof(params[0]);
  double x = 0;
  for (int i = 0; i < n; i++) {
    x += params[i] * theta[i];
  }
  Point inters(x, sidep1.y());
  return inters;
}

//

void BasicBehaviour::start() {
  for (int i = 0; i < 1000000; i++) {
    GS->updateState();
    int key = 0;
    key |= move();
    key |= shoot();
    GS->comm->sendCommand(key);
  }
}
