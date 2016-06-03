//
//  BasicBehaviour.hpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/26/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#ifndef BasicBehaviour_hpp
#define BasicBehaviour_hpp

#include "Constants.hpp"
#include "GlobalState.hpp"

class BasicBehaviour {
  GlobalState *GS;
  int move();
  int shoot();
  int shootToScore();
  int shootToClear();
  int defend();
  int moveToTarget(const Point &target);

  Vector predictShotAngle();
  Vector predictBounceAngle(Vector shotAngle);
  bool amClosestToBall();

  const double move_dx[9] = {0, 0,        SQRT1_2, 1,       SQRT1_2,
                             0, -SQRT1_2, -1,      -SQRT1_2};
  const double move_dy[9] = {0,  1,        SQRT1_2, 0,      -SQRT1_2,
                             -1, -SQRT1_2, 0,       SQRT1_2};
  const double move_key[9] = {0,
                              KEY_UP,
                              KEY_UP | KEY_RIGHT,
                              KEY_RIGHT,
                              KEY_DOWN | KEY_RIGHT,
                              KEY_DOWN,
                              KEY_DOWN | KEY_LEFT,
                              KEY_LEFT,
                              KEY_UP | KEY_LEFT};

public:
  void start();
  BasicBehaviour(GlobalState *GS);
};

#endif /* BasicBehaviour_hpp */
