//
//  GlobalState.hpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/27/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#ifndef GlobalState_hpp
#define GlobalState_hpp

#include "Communicator.hpp"

using std::string;

class Communicator;

class GlobalState {
  int myTeam;

  int score[2];

  char *parseConstants(char *message);
  char *parseScore(char *message);
  char *parseBall(char *message);
  char *parsePlayer(char *message, player *p);
  void initGoals();

public:
  Communicator *comm;
  double constPlayerRadius, constKickerRadius, constBallRadius, constPostRadius,
      constPostY, constMaxY, constMaxX;

  std::map<int, player> myPlayers;
  std::map<int, player> oppPlayers;
  player myPlayer;
  Point ballPos;
  Vector ballVel;
  goal myGoal;
  goal oppGoal;

  GlobalState(Communicator *comm);
  void updateState();
  int myScore();
  int oppScore();
};

#endif /* GlobalState_hpp */
