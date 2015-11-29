//
//  GlobalState.hpp
//  HaxClient
//
//  Created by Andrej Karadzic on 11/27/15.
//  Copyright © 2015 Andrej Karadzic. All rights reserved.
//

#ifndef GlobalState_hpp
#define GlobalState_hpp

#include <iostream>
#include <cstdlib>
#include <map>

using std::string;

struct coord {
  float x, y;
};

struct player {
  int id;
  coord pos;
  coord vel;

  bool const operator==(const player &that) { return (id == that.id); }
};

class Communicator;

class GlobalState {
  Communicator *comm;
  int myTeam;

  int score[2];

  char *parseScore(char *message);
  char *parseBall(char *message);
  char *parsePlayer(char *message, player *p);

public:
  std::map<int, player> myPlayers;
  std::map<int, player> oppPlayers;
  player myPlayer;
  coord ballPos;
  coord ballVel;

  GlobalState(Communicator *comm);
  void updateState();
  int myScore();
  int oppScore();
};

#endif /* GlobalState_hpp */
