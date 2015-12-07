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

public:
  void start();
  BasicBehaviour(GlobalState *GS);
};

#endif /* BasicBehaviour_hpp */
