//
//  BasicBehaviour.cpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#include "BasicBehaviour.hpp"

BasicBehaviour::BasicBehaviour(GlobalState *GS) : GS(GS) {}

bool BasicBehaviour::amClosestToBall() {
  Point p = GS->myPlayer.pos, b = GS->ballPos;

  double mDist = squared_distance(p, b);
  for (auto &kv : GS->oppPlayers) {
    double oppDist = squared_distance(kv.second.pos, b);
    if (oppDist < mDist) {
      return false;
    }
  }
  return true;
}

int BasicBehaviour::defend() {
  Point p = GS->myPlayer.pos, b = GS->ballPos;
  Point goal(GS->myGoal.x, 0);

  if (squared_distance(p, b) < squared_distance(p, goal))
    return moveToTarget(GS->ballPos);

  Line ballToGoal(GS->ballPos, goal);
  Point target = ballToGoal.projection(GS->myPlayer.pos);

  // If protecting the goal, start pressing
  if (squared_distance(p, target) < 1)
    return moveToTarget(b);

  return moveToTarget(target);
}

int BasicBehaviour::moveToTarget(const Point &target) {

  Point p = GS->myPlayer.pos;
  Vector pv = GS->myPlayer.vel;

  double minVal = 1e9;
  int choice = 0;

  for (int i = 0; i < 9; i++) {
    Vector move(move_dx[i] * 10, move_dy[i] * 10);

    Vector result = pv + move;

    Transformation scale(CGAL::SCALING, 0.1);
    result = scale(result);

    double dDist = squared_distance(p + result, target);

    if (dDist < minVal) {
      minVal = dDist;
      choice = i;
    }
  }
  return move_key[choice];
}

int BasicBehaviour::move() {

  Point p = GS->myPlayer.pos, b = GS->ballPos;

  if (amClosestToBall()) {
    double deltaMin = GS->constBallRadius + GS->constPlayerRadius + 1;
    if (squared_distance(p, b) > deltaMin * deltaMin) {
      return moveToTarget(b);
    }

    Transformation scale(CGAL::SCALING, .2);
    Point b2 = b + scale(GS->ballVel);

    Point oppGoal(GS->oppGoal.x, 0);
    Vector goalToBall(oppGoal, b2);

    double d = (GS->constBallRadius + GS->constPlayerRadius - .5) /
               sqrt(goalToBall.squared_length());
    scale = Transformation(CGAL::SCALING, d);

    Vector behindTheBall = scale(goalToBall);
    Vector sideOfTheBall1(-behindTheBall.y(), behindTheBall.x());
    Vector sideOfTheBall2(behindTheBall.y(), -behindTheBall.x());

    Line side1(b2 + behindTheBall, b2 + sideOfTheBall1);
    Line side2(b2 + behindTheBall, b2 + sideOfTheBall2);

    Circle player(GS->myPlayer.pos,
                  GS->constPlayerRadius * GS->constPlayerRadius);
    Point target;

    if (do_intersect(player, side1) || do_intersect(player, side2)) {
      target = b2 + behindTheBall;
    } else if (squared_distance(p, b2 + sideOfTheBall1) <
               squared_distance(p, b2 + sideOfTheBall2)) {
      target = b2 + sideOfTheBall1;
    } else {
      target = b2 + sideOfTheBall2;
    }

    return moveToTarget(target);
  } else {
    return defend();
  }
  return 0;
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

  Vector shootingVector = predictShotAngle();
  Ray shootingRay(GS->ballPos, shootingVector);

  auto result = intersection(shootingRay, goalLine);

  if (result)
    key |= KEY_SHOOT;
  else {
    if (signof(shootingVector.x()) == signof(GS->oppGoal.x)) {

      Point sidep1(3, GS->constMaxY * signof(shootingVector.y()));
      Point sidep2(4, GS->constMaxY * signof(shootingVector.y()));
      Line sideline(sidep1, sidep2);

      auto wallResult = intersection(shootingRay, sideline);
      Point wallInters = boost::get<Point>(wallResult.get());

      Vector bounceVector = predictBounceAngle(shootingVector);
      Ray bounceRay(wallInters, bounceVector);
      result = intersection(bounceRay, goalLine);
      if (result)
        key |= KEY_SHOOT;
    }
  }
  if (key) {
    std::cout << "sh: " << GS->ballVel.x() << " _:_ " << GS->ballVel.y()
              << std::endl;
  }
  return key;
}

Vector BasicBehaviour::predictShotAngle() {
  Point p = GS->myPlayer.pos, b = GS->ballPos;
  Vector bv = GS->ballVel, shv(p, b);

  double theta[] = {17.414298925323810, 1.507343274008287};

  double x = shv.x() * theta[0] + bv.x() * theta[1];
  double y = shv.y() * theta[0] + bv.y() * theta[1];
  Vector shot(x, y);
  return shot;
}

Vector BasicBehaviour::predictBounceAngle(Vector shotAngle) {

  double theta[] = {-0.600195312500000};

  double x = shotAngle.x();
  double y = shotAngle.y() * theta[0];
  Vector shot(x, y);
  return shot;
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
